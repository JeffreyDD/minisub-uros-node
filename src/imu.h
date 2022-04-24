#include <MPU9250_asukiaaa.h>

#define IMU_SDA_PIN 21
#define IMU_SCL_PIN 22

MPU9250_asukiaaa imu_sensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

void imu_setup()
{
  Wire.begin(IMU_SDA_PIN, IMU_SCL_PIN);
  imu_sensor.setWire(&Wire);

  imu_sensor.beginAccel();
  imu_sensor.beginGyro();
  imu_sensor.beginMag();
}

void imu_update()
{
  if (imu_sensor.accelUpdate() == 0) {
    aX = imu_sensor.accelX();
    aY = imu_sensor.accelY();
    aZ = imu_sensor.accelZ();
    aSqrt = imu_sensor.accelSqrt();
  } else {
    Serial.println("Cannod read accel values");
  }

  if (imu_sensor.gyroUpdate() == 0) {
    gX = imu_sensor.gyroX();
    gY = imu_sensor.gyroY();
    gZ = imu_sensor.gyroZ();
  } else {
    Serial.println("Cannot read gyro values");
  }

  if (imu_sensor.magUpdate() == 0) {
    mX = imu_sensor.magX();
    mY = imu_sensor.magY();
    mZ = imu_sensor.magZ();
    mDirection = imu_sensor.magHorizDirection();
  } else {
    Serial.println("Cannot read mag values"); 
  }
}