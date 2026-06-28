ESP32 Mini Drone — Technical Documentation

This project implements a simple ESP32-based mini drone control system using an MPU6050 inertial sensor, four PWM-controlled motors, and Wi-Fi remote control. The ESP32 reads the drone tilt, calculates pitch and roll corrections, and adjusts the motor outputs using PWM. It also creates a local Wi-Fi network, allowing the user to control the system from a phone or laptop through a simple browser page.

------------------------------------------------------------
Installation
------------------------------------------------------------

1. Install ESP32 board support

In Arduino IDE, open:
Tools -> Board -> Boards Manager

Search for:
esp32

Install:
esp32 by Espressif Systems

After installation, select your ESP32 board from:
Tools -> Board -> ESP32 Arduino -> ESP32 Dev Module

If you are using a specific ESP32 board, select that exact model instead.

2. Install the required libraries

In Arduino IDE, open:
Sketch -> Include Library -> Manage Libraries

Install the following libraries:
- Adafruit MPU6050
- Adafruit Unified Sensor
- Adafruit BusIO

The WiFi.h and WebServer.h libraries do not need to be installed separately. They are included automatically with the ESP32 board package.

------------------------------------------------------------
System Overview
------------------------------------------------------------

The project is divided into three main parts: sensor reading, stabilization logic, and wireless control. The MPU6050 sends acceleration data to the ESP32 through I2C. The ESP32 uses this data to estimate pitch and roll, then calculates correction values. These corrections are mixed with the user commands received through Wi-Fi and converted into PWM values for the four motors.

The Wi-Fi control is handled by the ESP32 itself. It creates its own access point, hosts a small web page, and receives commands such as W, A, S, D, Q, E, and X. This makes it possible to control the drone from a phone or laptop without needing an external router.

------------------------------------------------------------
Hardware Components
------------------------------------------------------------

Required components:
- ESP32 development board
- MPU6050 / GY-521 module
- 4 DC motors
- 4 transistors, MOSFETs, or motor drivers
- External power supply for the motors
- Jumper wires
- Breadboard or prototype board

The ESP32 should only be used to send control signals. The motors must be powered separately through proper motor-driving components.

------------------------------------------------------------
MPU6050 Wiring
------------------------------------------------------------

The MPU6050 communicates with the ESP32 using I2C.

MPU6050 -> ESP32
VCC -> 3V3
GND -> GND
SDA -> GPIO21
SCL -> GPIO22
AD0 -> GND

When AD0 is connected to GND, the MPU6050 I2C address is 0x68.

------------------------------------------------------------
Motor Wiring
------------------------------------------------------------

The motors must not be connected directly to the ESP32 pins. Each ESP32 GPIO pin sends a PWM signal to a transistor, MOSFET, or motor driver, which then controls the motor.

Motor control pin mapping:
- GPIO15 -> Motor 1, front-left
- GPIO18 -> Motor 2, front-right
- GPIO19 -> Motor 3, back-left
- GPIO23 -> Motor 4, back-right

Basic motor connection:
ESP32 GPIO -> transistor / MOSFET / motor driver -> motor

The external motor power supply must power the motors, and its ground must be connected to the ESP32 ground:
Motor power supply GND -> ESP32 GND

------------------------------------------------------------
Motor Layout
------------------------------------------------------------

              FRONT

        Motor 1     Motor 2
        GPIO15      GPIO18

        Motor 3     Motor 4
        GPIO19      GPIO23

              BACK

------------------------------------------------------------
Wi-Fi Control
------------------------------------------------------------

The ESP32 creates its own Wi-Fi network.

Network name:
DroneESP32

Password:
12345678

After connecting to this network from a phone or laptop, open the ESP32 IP address in a browser. In most cases, the address is:
192.168.4.1

The web page sends simple movement commands to the ESP32.

Available commands:
- W = forward
- S = backward
- A = left
- D = right
- Q = decrease throttle
- E = increase throttle
- X = stop motors

The commands do not directly power the motors. They modify control variables inside the program, such as throttle, pitchCmd, and rollCmd. These variables are then combined with the stabilization corrections.

------------------------------------------------------------
Project Files
------------------------------------------------------------

minidrona.ino

This is the main control file. It initializes the MPU6050, reads sensor data, calculates pitch and roll, computes stabilization corrections, mixes the user commands with the correction values, and sends PWM signals to the four motors.

wifi_control.ino

This file handles the wireless control system. It creates the ESP32 Wi-Fi access point, starts a WebServer on port 80, serves a simple HTML control page, receives W, A, S, D, Q, E, and X commands, and updates the global control variables used by the main file.

------------------------------------------------------------
Sensor Processing
------------------------------------------------------------

The MPU6050 provides acceleration values on three axes: ax, ay, and az. The ESP32 uses these values to estimate the tilt of the drone.

Pitch represents forward/backward tilt.
Roll represents left/right tilt.

The target position is pitch = 0 and roll = 0, meaning the drone is considered level. The error is calculated as the difference between the target value and the measured value.

Formula:
error = target value - measured value

The correction is calculated using a proportional gain called Kp.

Formula:
correction = error × Kp

A higher Kp makes the motors react more strongly to tilt. A lower Kp makes the system react more slowly and smoothly.

------------------------------------------------------------
Motor Control Logic
------------------------------------------------------------

The final PWM value for each motor is calculated by combining the base throttle, the user command, and the stabilization correction.

Main control variables:
- throttle controls the base motor power
- pitchCmd is modified by W and S commands
- rollCmd is modified by A and D commands
- pitchCorrection comes from the MPU6050 pitch error
- rollCorrection comes from the MPU6050 roll error

The resulting values are limited to the PWM range of 0 to 255. A value of 0 means the motor is stopped, while 255 means maximum PWM output.

------------------------------------------------------------
PWM Output
------------------------------------------------------------

The ESP32 uses LEDC PWM output for motor control.

PWM settings:
- Frequency: 1000 Hz
- Resolution: 8-bit
- Value range: 0 to 255

Because the resolution is 8-bit, the PWM output accepts values from 0 to 255. These values control the average power sent to the motor driver.

------------------------------------------------------------
Important Notes
------------------------------------------------------------

This project is a simplified educational prototype, not a complete flight controller. It is meant to demonstrate how an ESP32 can read MPU6050 motion data, calculate basic pitch and roll corrections, receive Wi-Fi commands, and generate PWM signals for motor control.

For real flight, the system would need PID control, sensor calibration, signal filtering, failsafe logic, battery monitoring, emergency stop handling, proper motor drivers, and a safe mechanical design.

Always test without propellers first. Do not connect motors directly to the ESP32 pins. Each motor must be driven through a transistor, MOSFET, or motor driver, and the motor power supply ground must be connected to the ESP32 ground.
