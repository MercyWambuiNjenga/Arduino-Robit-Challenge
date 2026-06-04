/* 
 * Calibration Utility
 * Run this to measure the encoder clicks per 2 meters for your specific wheels.
 * Push the robot by hand and watch the Serial Monitor!
 */

// 1. Define the pins
#define LEFT_ENCODER_PIN  2
#define RIGHT_ENCODER_PIN 3

// 2. Create the memory variables (volatile because they use interrupts)
volatile unsigned long leftEncoderTicks = 0;
volatile unsigned long rightEncoderTicks = 0;

// 3. Create the Interrupt Service Routines
void countLeft() {
  leftEncoderTicks++;
}

void countRight() {
  rightEncoderTicks++;
}

void setup() {
  // Start the computer screen connection
  Serial.begin(9600);
  
  // Setup the pins
  pinMode(LEFT_ENCODER_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER_PIN, INPUT_PULLUP);
  
  // Attach the interrupts
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_PIN), countLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_PIN), countRight, RISING);

  Serial.println("Calibration Utility Ready!");
  Serial.println("Push the robot 2 meters now...");
}

void loop() {
  // Print the clicks to the computer screen!
  Serial.print("Left Clicks: ");
  Serial.print(leftEncoderTicks);
  Serial.print(" | Right Clicks: ");
  Serial.println(rightEncoderTicks);
  
  delay(100); // Slow down the printing so it's easy to read
}
