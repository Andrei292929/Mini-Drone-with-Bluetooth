#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

#define MOTOR_FS 19  // F-S
#define MOTOR_FD 17  // F-D
#define MOTOR_SS 23  // S-S
#define MOTOR_SD 15  // S-D

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

  ledcAttach(MOTOR_FS, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_FD, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_SS, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR_SD, PWM_FREQ, PWM_RES);

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

  int motorFSValue = throttle + pitchCmd + rollCmd - pitchCorrection - rollCorrection;
  int motorFDValue = throttle + pitchCmd - rollCmd - pitchCorrection + rollCorrection;
  int motorSSValue = throttle - pitchCmd + rollCmd + pitchCorrection - rollCorrection;
  int motorSDValue = throttle - pitchCmd - rollCmd + pitchCorrection + rollCorrection;

  motorFSValue = constrain(motorFSValue, 0, 255);
  motorFDValue = constrain(motorFDValue, 0, 255);
  motorSSValue = constrain(motorSSValue, 0, 255);
  motorSDValue = constrain(motorSDValue, 0, 255);

  ledcWrite(MOTOR_FS, motorFSValue);
  ledcWrite(MOTOR_FD, motorFDValue);
  ledcWrite(MOTOR_SS, motorSSValue);
  ledcWrite(MOTOR_SD, motorSDValue);


  //Pt debug:
  // Serial.print("Throttle: ");
  // Serial.print(throttle);

  // Serial.print(" | Pitch: ");
  // Serial.print(pitch);

  // Serial.print(" | Roll: ");
  // Serial.print(roll);

  // Serial.print(" | pitchCmd: ");
  // Serial.print(pitchCmd);

  // Serial.print(" | rollCmd: ");
  // Serial.print(rollCmd);

  // Serial.print(" | FS: ");
  // Serial.print(motorFSValue);

  // Serial.print(" FD: ");
  // Serial.print(motorFDValue);

  // Serial.print(" SS: ");
  // Serial.print(motorSSValue);

  // Serial.print(" SD: ");
  // Serial.println(motorSDValue);

  delay(100);
}
