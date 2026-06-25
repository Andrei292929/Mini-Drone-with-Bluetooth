#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

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

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t accel, gyro, temp;

  mpu.getEvent(&accel, &gyro, &temp);

  Serial.print("Gyro X: ");
  Serial.print(gyro.gyro.x * 57.2958);
  Serial.print("  Y: ");
  Serial.print(gyro.gyro.y * 57.2958);
  Serial.print("  Z: ");
  Serial.print(gyro.gyro.z * 57.2958);
  Serial.println(" deg/s");

  delay(300);
}