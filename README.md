README — ESP32 Mini Drone with MPU6050 and Wi-Fi Control

Project Description

This project uses an ESP32, an MPU6050 sensor, and 4 motors to create a simple mini drone control system.

The ESP32 reads the tilt of the drone using the MPU6050, calculates pitch and roll corrections, and controls the motors using PWM.

The drone can also be controlled through Wi-Fi. The ESP32 creates its own Wi-Fi network, and the user can connect with a phone or laptop. Commands are sent from a simple web page using W, A, S, D keys.

Components

ESP32
MPU6050 / GY-521
4 motors
4 transistors / MOSFETs / motor drivers
External power supply for motors
Jumper wires

MPU6050 Connections

MPU6050 VCC -> ESP32 3V3
MPU6050 GND -> ESP32 GND
MPU6050 SDA -> ESP32 GPIO21
MPU6050 SCL -> ESP32 GPIO22
MPU6050 AD0 -> GND

Motor Connections

GPIO15 -> Motor 1, front-left
GPIO18 -> Motor 2, front-right
GPIO19 -> Motor 3, back-left
GPIO23 -> Motor 4, back-right

Motors must not be connected directly to the ESP32. Use transistors, MOSFETs, or motor drivers.

The motor power supply GND must be connected to the ESP32 GND.

Simple Wiring Diagram

                 +----------------+
                 |     ESP32      |
                 |                |
        GPIO21 --| SDA        3V3 |-- VCC MPU6050
        GPIO22 --| SCL        GND |-- GND MPU6050
                 |                |
        GPIO15 --| PWM M1         |
        GPIO18 --| PWM M2         |
        GPIO19 --| PWM M3         |
        GPIO23 --| PWM M4         |
                 +----------------+
              GND | 
                  |
                  +-------------------- GND motor power supply


                 +----------------+
                 |    MPU6050     |
                 |                |
                 | VCC -> 3V3     |
                 | GND -> GND     |
                 | SDA -> GPIO21  |
                 | SCL -> GPIO22  |
                 | AD0 -> GND     |
                 +----------------+


Motor Layout

                  FRONT

          Motor 1        Motor 2
          GPIO15         GPIO18


          Motor 3        Motor 4
          GPIO19         GPIO23

                  BACK


Motor Driver Concept

ESP32 GPIO pin -> transistor / MOSFET / motor driver -> motor

External motor power supply -> motors
External motor power supply GND -> ESP32 GND

Wi-Fi Control

The ESP32 creates a Wi-Fi network:

SSID: DroneESP32
Password: 12345678

After connecting to this network, open the ESP32 IP address in a browser. Usually:

192.168.4.1

Controls:

W = forward
S = backward
A = left
D = right
Q = decrease throttle
E = increase throttle
X = stop motors

Required Libraries

Install from Boards Manager:

esp32 by Espressif Systems

Install from Library Manager:

Adafruit MPU6050
Adafruit Unified Sensor
Adafruit BusIO

WiFi.h and WebServer.h are included with the ESP32 board package.

How It Works

The MPU6050 measures the drone tilt.

The ESP32 calculates:

pitch = forward/backward tilt
roll = left/right tilt

The target position is:

pitch = 0
roll = 0

The correction is calculated using:

correction = error × Kp

The final motor values are calculated from:

throttle
user command
stabilization correction

Project Files

minidrona.ino

Handles MPU6050 reading, pitch/roll calculation, stabilization, and motor PWM output.

wifi_control.ino

Creates the Wi-Fi network, hosts the control web page, and receives W A S D Q E X commands.

Warning

This is a simplified educational project, not a complete flight controller.

Test without propellers first.

Do not connect motors directly to the ESP32.
