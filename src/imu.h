#ifndef IMU_H_
#define IMU_H_

#include <MPU9250_asukiaaa.h>

#define IMU_SDA_PIN 15
#define IMU_SCL_PIN 14

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

void euler_to_quat(float x, float y, float z, double* q) {
    float c1 = cos((y*3.14/180.0)/2);
    float c2 = cos((z*3.14/180.0)/2);
    float c3 = cos((x*3.14/180.0)/2);

    float s1 = sin((y*3.14/180.0)/2);
    float s2 = sin((z*3.14/180.0)/2);
    float s3 = sin((x*3.14/180.0)/2);

    q[0] = c1 * c2 * c3 - s1 * s2 * s3;
    q[1] = s1 * s2 * c3 + c1 * c2 * s3;
    q[2] = s1 * c2 * c3 + c1 * s2 * s3;
    q[3] = c1 * s2 * c3 - s1 * c2 * s3;
}

#endif