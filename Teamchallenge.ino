/*
  ENG109 Engineering Problem Solving and Data Analysis
  Project: Arduino Autonomous Vehicle
  
This code contains design for an autonomous vehicle supposed to navigate a 2-meter course while identifying and avoiding obstacles.
*/

// Drive pin definitions
#define DRIVE_RIGHT_PIN_1 9   
#define DRIVE_RIGHT_PIN_2 8
#define DRIVE_LEFT_PIN_1  10  
#define DRIVE_LEFT_PIN_2  12

// Encoder pin definitions:
#define LEFT_ENCODER_PIN  2
#define RIGHT_ENCODER_PIN 3

// Ultrasonic sensors:
#define LEFT_TRIG_PIN     A1
#define LEFT_ECHO_PIN     A0
#define RIGHT_TRIG_PIN    A2
#define RIGHT_ECHO_PIN    A3

// RGB LEDs:
#define RED_LED_PIN           5
#define GREEN_LED_PIN         11
#define BLUE_LED_PIN          6
#define LED_LEFT_EN_PIN       4
#define LED_RIGHT_EN_PIN      7

#define LED_ENABLED   LOW
#define LED_DISABLED  HIGH

// --- 2-Meter Goal ---
volatile unsigned long leftEncoderTicks = 0;
volatile unsigned long rightEncoderTicks = 0;
volatile bool isTurning = false; 

// 250 Ticks is roughly 2 meters for a 65mm wheel! (2000 was 20 meters!)
const unsigned long TARGET_TICKS = 250; 

unsigned long startTime = 0; 

// --- Interrupt Service Routines ---
void countLeft() { if (!isTurning) leftEncoderTicks++; }
void countRight() { if (!isTurning) rightEncoderTicks++; }

void setup()
{
  pinMode(DRIVE_RIGHT_PIN_1, OUTPUT);
  pinMode(DRIVE_RIGHT_PIN_2, OUTPUT);
  pinMode(DRIVE_LEFT_PIN_1, OUTPUT);
  pinMode(DRIVE_LEFT_PIN_2, OUTPUT);
  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(LED_LEFT_EN_PIN, OUTPUT);
  pinMode(LED_RIGHT_EN_PIN, OUTPUT); 
  
  pinMode(LEFT_TRIG_PIN, OUTPUT);  
  pinMode(LEFT_ECHO_PIN, INPUT);   
  pinMode(RIGHT_TRIG_PIN, OUTPUT); 
  pinMode(RIGHT_ECHO_PIN, INPUT);  
  
  pinMode(LEFT_ENCODER_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_PIN, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), countLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), countRight, RISING);

  Serial.begin(9600); 

  // 1-Second Warmup
  setLED(HIGH, HIGH, LOW); 
  delay(1000);
  
  // Record the exact time the robot actually starts driving
  startTime = millis(); 
}

void loop()
{
  // 1. Check if 2-meter goal reached OR 10 seconds have passed (Emergency Stop)
  if (leftEncoderTicks >= TARGET_TICKS || rightEncoderTicks >= TARGET_TICKS || millis() - startTime > 10000) {
    isTurning = true;         
    setMotors(0, 0);          
    setLED(LOW, LOW, HIGH);   // BLUE = Goal Reached / Stopped
    return;                   
  }

  // 2. TIME-BASED BLIND START (Drives straight for 1 second out of the start box)
  if (millis() - startTime < 1000) {
     isTurning = false;
     setMotors(1, 1);
     setLED(LOW, HIGH, LOW);  // GREEN
     return;                  
  }

  // 3. Read Sensors
  long leftDist = readDistance(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
  long rightDist = readDistance(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
  
  // 4. Obstacle Avoidance 
  if (leftDist < 20 || rightDist < 20) {
    
    isTurning = true;         // Pause 2-meter counter during the dodge
    setLED(HIGH, LOW, LOW);   // RED LED
    
    // DECISION: Which way do we dodge?
    if (rightDist < 20) {
      // Box is on the RIGHT. We must dodge LEFT.
      setMotors(-1, 1);       // 1. Turn Left
      delay(300);             //    (Adjust this so it turns about 45 degrees)
      
      setMotors(1, 1);        // 2. Drive straight past the box
      delay(600);             //    (Adjust this to make sure it clears the box)
      
      setMotors(1, -1);       // 3. Turn Right to face the finish line again!
      delay(300);             //    (Must be the same time as step 1 to straighten out)
    } 
    else {
      // Box is on the LEFT (or Dead Center). We must dodge RIGHT.
      setMotors(1, -1);       // 1. Turn Right
      delay(300);             
      
      setMotors(1, 1);        // 2. Drive straight past the box
      delay(600);             
      
      setMotors(-1, 1);       // 3. Turn Left to face the finish line again!
      delay(300);             
    }
  } 
  else {
    // Path is clear
    isTurning = false;        // Turn counting back on!
    setMotors(1, 1);          // Drive straight to the finish line
    setLED(LOW, HIGH, LOW);   // GREEN LED
  }
}

// --- Helper Functions ---

void setLED(bool red, bool green, bool blue) {
  digitalWrite(RED_LED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_LED_PIN, green ? HIGH : LOW);
  digitalWrite(BLUE_LED_PIN, blue ? HIGH : LOW);
  digitalWrite(LED_LEFT_EN_PIN, LED_ENABLED);
  digitalWrite(LED_RIGHT_EN_PIN, LED_ENABLED);
}

// ORIGINAL WORKING MOTOR CODE
void setMotors(int leftDir, int rightDir) {
  
  // Left Motor
  if (leftDir == 1) { // Forward
    digitalWrite(DRIVE_LEFT_PIN_1, LOW);
    digitalWrite(DRIVE_LEFT_PIN_2, HIGH);
  } else if (leftDir == -1) { // Reverse (For Turning)
    digitalWrite(DRIVE_LEFT_PIN_1, HIGH);
    digitalWrite(DRIVE_LEFT_PIN_2, LOW);
  } else { // Stop
    digitalWrite(DRIVE_LEFT_PIN_1, LOW);
    digitalWrite(DRIVE_LEFT_PIN_2, LOW);
  }
  
  // Right Motor
  if (rightDir == 1) { // Forward
    digitalWrite(DRIVE_RIGHT_PIN_1, LOW);
    digitalWrite(DRIVE_RIGHT_PIN_2, HIGH);
  } else if (rightDir == -1) { // Reverse (For Turning)
    digitalWrite(DRIVE_RIGHT_PIN_1, HIGH);
    digitalWrite(DRIVE_RIGHT_PIN_2, LOW);
  } else { // Stop
    digitalWrite(DRIVE_RIGHT_PIN_1, LOW);
    digitalWrite(DRIVE_RIGHT_PIN_2, LOW);
  }
}

long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); 
  if (duration == 0) { return 999; }
  return duration / 58; 
}
