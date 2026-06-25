README — ESP32 Stabilization Project

This project uses an ESP32, an MPU6050 sensor, and four motors to create a basic stabilization system. The MPU6050 detects the board’s pitch and roll angles, and the ESP32 adjusts the speed of each motor using PWM to help keep the system level.

Components:
- ESP32
- MPU6050 / GY-521
- 4 motors
- 4 transistors / MOSFETs / motor drivers
- External power supply for the motors
- Jumper wires

MPU6050 connections:

MPU6050 VCC -> ESP32 3V3
MPU6050 GND -> ESP32 GND
MPU6050 SDA -> ESP32 GPIO21
MPU6050 SCL -> ESP32 GPIO22
MPU6050 AD0 -> GND

Motor connections:

GPIO15 -> MOTOR1 front-left
GPIO17 -> MOTOR2 front-right
GPIO19 -> MOTOR3 back-left
GPIO23 -> MOTOR4 back-right

Motor layout:

              FRONT

        MOTOR1        MOTOR2
        GPIO15        GPIO17
      front-left   front-right


        MOTOR3        MOTOR4
        GPIO19        GPIO23
       back-left    back-right

              BACK

How it works:

MPU6050 reads movement
        |
        v
ESP32 calculates pitch and roll
        |
        v
ESP32 calculates correction
        |
        v
PWM signals control the 4 motors

Control idea:

The target position is:

pitch = 0
roll  = 0

The correction is calculated using:

error = target angle - measured angle
correction = error × Kp

The variable baseSpeed sets the default motor speed.
The variable Kp controls how strongly the motors react to tilt.

Important:

Do not connect the motors directly to the ESP32 pins.
Use transistors, MOSFETs, or motor drivers.
The motor power supply GND must be connected to the ESP32 GND.

This project is a simple starting point for learning motor control, I2C communication, PWM, and basic stabilization with an inertial sensor.
