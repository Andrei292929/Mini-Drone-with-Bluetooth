#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Motoare
#define MOTOR1 15  // fata stanga
#define MOTOR2 18  // fata dreapta
#define MOTOR3 19  // spate stanga
#define MOTOR4 23  // spate dreapta

#define PWM_FREQ 1000
#define PWM_RES 8   // 8 biti: valori 0 - 255

int baseSpeed = 120;
float Kp = 5.0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(21, 22); // SDA = 21, SCL = 22

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

//pwm
  ledcAttach(MOTOR1, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR2, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR3, PWM_FREQ, PWM_RES);
  ledcAttach(MOTOR4, PWM_FREQ, PWM_RES);

  Serial.println("PWM motoare pornit.");
}

void loop() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float ax = accel.acceleration.x;
  float ay = accel.acceleration.y;
  float az = accel.acceleration.z;


  float roll  = atan2(ay, az) * 180.0 / PI;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

  float targetPitch = 0.0;
  float targetRoll = 0.0;

  float pitchError = targetPitch - pitch;
  float rollError  = targetRoll - roll;

  float pitchCorrection = pitchError * Kp;
  float rollCorrection  = rollError * Kp;

  int motor1Value = baseSpeed + pitchCorrection + rollCorrection;
  int motor2Value = baseSpeed + pitchCorrection - rollCorrection; 
  int motor3Value = baseSpeed - pitchCorrection + rollCorrection;
  int motor4Value = baseSpeed - pitchCorrection - rollCorrection; 

  motor1Value = constrain(motor1Value, 0, 255);
  motor2Value = constrain(motor2Value, 0, 255);
  motor3Value = constrain(motor3Value, 0, 255);
  motor4Value = constrain(motor4Value, 0, 255);

  ledcWrite(MOTOR1, motor1Value);
  ledcWrite(MOTOR2, motor2Value);
  ledcWrite(MOTOR3, motor3Value);
  ledcWrite(MOTOR4, motor4Value);

  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" Roll: ");
  Serial.print(roll);

  Serial.print(" | MOTOR1: ");
  Serial.print(motor1Value);
  Serial.print(" MOTOR2: ");
  Serial.print(motor2Value);
  Serial.print(" MOTOR3: ");
  Serial.print(motor3Value);
  Serial.print(" MOTOR4: ");
  Serial.println(motor4Value);

  delay(100);
}