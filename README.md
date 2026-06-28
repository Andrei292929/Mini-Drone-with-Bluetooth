ESP32 Mini Drone — Technical Documentation

This project implements a simple ESP32-based mini drone control system using an MPU6050 inertial sensor, four PWM-controlled motors, and Wi-Fi remote control. The ESP32 reads the drone tilt using the MPU6050, calculates pitch and roll corrections, and adjusts the motor outputs using PWM. It also creates a local Wi-Fi network, allowing the user to control the drone from a phone or laptop through a simple browser interface.

Installation
1. Install ESP32 board support:
Arduino IDE -> Tools -> Board -> Boards Manager
Search for: esp32
Install: esp32 by Espressif Systems
Then select your board from:
Tools -> Board -> ESP32 Arduino -> ESP32 Dev Module
or choose the exact ESP32 board you are using.

2. Install required libraries:
Arduino IDE -> Sketch -> Include Library -> Manage Libraries
Install:
- Adafruit MPU6050
- Adafruit Unified Sensor
- Adafruit BusIO

WiFi.h and WebServer.h do not need to be installed separately. They are included with the ESP32 board package.

System Architecture
 ┌──────────────────────────────────────────────────────┐
 │                    ESP32 MAIN LOOP                   │
 │  - Reads Wi-Fi commands                              │
 │  - Reads MPU6050 sensor data                         │
 │  - Calculates pitch and roll                         │
 │  - Computes motor correction values                  │
 │  - Sends PWM signals to motor drivers                │
 └───────────────┬──────────────────────────────────────┘
                 │
                 ▼
       ┌────────────────────┐
       │      MPU6050       │
       │  Accelerometer     │
       │  Gyroscope         │
       └────────────────────┘
                 │ I2C
                 ▼
       ┌────────────────────┐
       │   Stabilization    │
       │  Pitch/Roll logic  │
       └────────────────────┘
                 │ PWM values
                 ▼
       ┌────────────────────────────────────┐
       │      Motor Driver Stage            │
       │  - Transistors / MOSFETs / drivers │
       │  - External motor power supply     │
       │  - Common GND with ESP32           │
       └────────────────────────────────────┘

Wi-Fi Control
 ┌────────────────────────────────────┐
 │          Wi-Fi Control             │
 │  - ESP32 Access Point              │
 │  - WebServer on port 80            │
 │  - Browser-based W A S D control   │
 └────────────────────────────────────┘

Hardware Components
- ESP32
- MPU6050 / GY-521
- 4 DC motors
- 4 transistors / MOSFETs / motor drivers
- External power supply for motors
- Jumper wires
- Breadboard or prototype board

MPU6050 Wiring
MPU6050 VCC -> ESP32 3V3
MPU6050 GND -> ESP32 GND
MPU6050 SDA -> ESP32 GPIO21
MPU6050 SCL -> ESP32 GPIO22
MPU6050 AD0 -> GND

With AD0 connected to GND, the MPU6050 I2C address is 0x68.

Motor Wiring
The motors must not be connected directly to the ESP32 pins. The ESP32 only provides PWM control signals. Each motor must be driven using a transistor, MOSFET, or motor driver.

GPIO15 -> Motor 1, front-left
GPIO18 -> Motor 2, front-right
GPIO19 -> Motor 3, back-left
GPIO23 -> Motor 4, back-right

The motor power supply GND must be connected to the ESP32 GND.

Motor Layout
                  FRONT
          Motor 1        Motor 2
          GPIO15         GPIO18

          Motor 3        Motor 4
          GPIO19         GPIO23
                  BACK

Wi-Fi Control
The ESP32 creates its own Wi-Fi network:
SSID: DroneESP32
Password: 12345678

After connecting to this network, open the ESP32 IP address in a browser. Usually:
192.168.4.1

Available commands:
W = forward
S = backward
A = left
D = right
Q = decrease throttle
E = increase throttle
X = stop motors

Project Files
minidrona.ino:
- Initializes the MPU6050
- Reads accelerometer data
- Calculates pitch and roll
- Computes stabilization corrections
- Mixes user commands with stabilization output
- Sends PWM signals to the four motors

wifi_control.ino:
- Creates the ESP32 Wi-Fi access point
- Starts a WebServer on port 80
- Serves a simple HTML control page
- Receives W A S D Q E X commands
- Updates throttle, pitchCmd, and rollCmd

Sensor Processing
The MPU6050 provides acceleration values on three axes: ax, ay, az.
The ESP32 uses these values to estimate:
pitch = forward/backward tilt
roll = left/right tilt

The target position is:
pitch = 0
roll = 0

The error is calculated as:
error = target value - measured value

The correction is calculated as:
correction = error × Kp

Kp is the proportional gain. A higher Kp makes the motors react more strongly to tilt.

Motor Control Logic
The final motor values are calculated using:
- throttle
- pitchCmd
- rollCmd
- pitchCorrection
- rollCorrection

throttle controls the base motor power.
pitchCmd is modified by W and S commands.
rollCmd is modified by A and D commands.
pitchCorrection and rollCorrection come from the MPU6050 stabilization logic.
The resulting motor values are limited to the PWM range 0 to 255.

PWM Output
The ESP32 uses LEDC PWM output.
Frequency: 1000 Hz
Resolution: 8-bit
Range: 0–255

0 means motor off.
255 means maximum PWM output.

Important Notes
This is a simplified educational prototype, not a complete flight controller. For real flight, additional features are required:
- PID control
- Sensor calibration
- Filtering
- Failsafe logic
- Battery monitoring
- Emergency stop
- Proper motor drivers
- Safe mechanical design

Always test without propellers first. Do not connect motors directly to the ESP32.
