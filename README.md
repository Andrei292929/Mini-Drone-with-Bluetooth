README — ESP32 + MPU6050 + 4 motoare

Proiectul folosește un ESP32 și un senzor MPU6050 pentru a controla 4 motoare prin PWM. Sistemul citește înclinarea plăcii și modifică viteza motoarelor pentru a încerca să mențină placa dreaptă.

Componente:

ESP32
MPU6050 / GY-521
4 motoare
4 tranzistori / MOSFET-uri / drivere motor
Sursă externă pentru motoare
Fire jumper

Conexiuni MPU6050:

MPU6050 VCC -> ESP32 3V3
MPU6050 GND -> ESP32 GND
MPU6050 SDA -> ESP32 GPIO21
MPU6050 SCL -> ESP32 GPIO22
MPU6050 AD0 -> GND

Adresa senzorului este 0x68.

Conexiuni motoare:

GPIO15 -> MOTOR1 față-stânga
GPIO18 -> MOTOR2 față-dreapta
GPIO19 -> MOTOR3 spate-stânga
GPIO23 -> MOTOR4 spate-dreapta

Motoarele nu se conectează direct la ESP32. Pinii ESP32 controlează tranzistori, MOSFET-uri sau drivere de motor.

GND-ul sursei motoarelor trebuie conectat la GND-ul ESP32.

Librării necesare:

Adafruit MPU6050
Adafruit Unified Sensor
Adafruit BusIO
