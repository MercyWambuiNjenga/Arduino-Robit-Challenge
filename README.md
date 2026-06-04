Autonomous Vehicle Control System (ENG109)
Project Overview
This repository contains the firmware for an autonomous vehicle designed to navigate a 2-meter course while identifying and avoiding obstacles. The system is built on the Arduino UNO R4 platform, focusing on high-reliability decision-making and precise position estimation.

Key Objectives & Features
Position Estimation: Utilizes wheel encoders and hardware interrupts for real-time distance tracking to hit the 2-meter mark accurately.
Obstacle Avoidance: Dual-sensor ultrasonic array provides 180-degree forward coverage with immediate emergency stop logic.
Visual Feedback: RGB LED system provides real-time state indication (Green: Moving, Red: Obstacle, Blue: Goal Reached).
Computational Efficiency: Uses 
d
=
t
/
58
 for lightweight sensor processing and Boolean logic for rapid decision-making.
Hardware Components
Microcontroller: Arduino UNO R4 Minima
Motor Driver: L298N Dual H-Bridge
Sensors: 2x HC-SR04 Ultrasonic Sensors, 2x Wheel Encoders
Indicators: Common Cathode RGB LEDs
How to Run
Phase 1: Hardware Calibration (Measuring the Wheels)
Because physical wheel sizes and motor configurations vary, the system must be calibrated to your specific robot before running the main code.

Connect the Arduino to your computer via USB.
Upload the Calibration.ino sketch to the Arduino.
Open the Arduino IDE Serial Monitor (set to 9600 baud).
Place the robot at a starting line and manually push it forward exactly 2 meters.
Look at the Serial Monitor and record the final encoder tick count (e.g., Left Clicks: 450).
Open the main TeamChallenge.ino file and update line 44 with your recorded number: const unsigned long TARGET_TICKS = 450;
Phase 2: Execution (The 2-Meter Run)
Upload the updated TeamChallenge.ino to the Arduino.
Safety Check: Hold the robot securely in the air so the wheels don't touch the desk. Place your hand in front of the ultrasonic sensors to ensure the wheels stop (Red LED) and resume when clear (Green LED).
Ensure the 9V power supply is securely connected to the L298N Motor Driver.
Place the robot at the start of the 2-meter track.
The robot will autonomously navigate, avoid obstacles, and halt automatically at the 2-meter mark (indicated by the Blue LED).

