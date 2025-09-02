#include <SPI.h>
#include "RF24.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int data[2];
RF24 radio(8, 9);
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection())
  {
    Serial.println("MPU6050 connection successful.");
  }
  else
  {
    Serial.println("MPU6050 connection failed.");
  }

  radio.begin();
  radio.openWritingPipe(pipe);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop()
{
  unsigned long lastTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= 100)
  {
    lastTime = currentTime;

    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    data[0] = constrain(map(ax, -17000, 17000, 300, 400), 300, 400);
    data[1] = constrain(map(ay, -17000, 17000, 100, 200), 100, 200);

    bool success = radio.write(data, sizeof(data));

    if (success)
    {
      Serial.print("X-axis: ");
      Serial.println(data[0]);
      Serial.print("Y-axis: ");
      Serial.println(data[1]);
    }
    else
    {
      Serial.println("Data transmission failed.");
    }
  }
