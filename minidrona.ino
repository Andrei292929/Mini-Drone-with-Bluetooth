#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

#define MOTOR1 15
#define MOTOR2 18
#define MOTOR3 19
#define MOTOR4 23

#define PWM_FREQ 1000
#define PWM_RES 8

int throttle = 120;
int pitchCmd = 0;
int rollCmd = 0;

float Kp = 5.0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22);

  if (!mpu.begin(0x68)) {
    Serial.println("Nu am gasit MPU6050!");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 gasit!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  ledcAttach(MOTOR1, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR2, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR3, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR4, PWM_FREQ, PWM_RES);

  setupWiFiControl();

  Serial.println("Sistem pornit.");
}

void loop() {
  readWiFiControl();

  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float ax = accel.acceleration.x;
  float ay = accel.acceleration.y;
  float az = accel.acceleration.z;

  float roll  = atan2(ay, az) * 180.0 / PI;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

  float pitchError = 0 - pitch;
  float rollError  = 0 - roll;

  float pitchCorrection = pitchError * Kp;
  float rollCorrection  = rollError * Kp;

  int motor1Value = throttle + pitchCmd - rollCmd + pitchCorrection - rollCorrection;
  int motor2Value = throttle + pitchCmd + rollCmd + pitchCorrection + rollCorrection;
  int motor3Value = throttle - pitchCmd - rollCmd - pitchCorrection - rollCorrection;
  int motor4Value = throttle - pitchCmd + rollCmd - pitchCorrection + rollCorrection;

  motor1Value = constrain(motor1Value, 0, 255);
  motor2Value = constrain(motor2Value, 0, 255);
  motor3Value = constrain(motor3Value, 0, 255);
  motor4Value = constrain(motor4Value, 0, 255);

  ledcWrite(MOTOR1, motor1Value);
  ledcWrite(MOTOR2, motor2Value);
  ledcWrite(MOTOR3, motor3Value);
  ledcWrite(MOTOR4, motor4Value);

  // Serial.print("Throttle: ");
  // Serial.print(throttle);

  // Serial.print(" Pitch: ");
  // Serial.print(pitch);

  // Serial.print(" Roll: ");
  // Serial.print(roll);

  // Serial.print(" M1: ");
  // Serial.print(motor1Value);

  // Serial.print(" M2: ");
  // Serial.print(motor2Value);

  // Serial.print(" M3: ");
  // Serial.print(motor3Value);

  // Serial.print(" M4: ");
  // Serial.println(motor4Value);

  delay(100);
}