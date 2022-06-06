#ifndef IMU_H_
#define IMU_H_

#include "eigen.h"
#include "mpu9250.h"

/* Mpu9250 object */
bfs::Mpu9250 imu;

#define IMU_SDA_PIN 15
#define IMU_SCL_PIN 14

float accel_x_mps2, accel_y_mps2, accel_z_mps2;
Eigen::Vector3f accel_mps2;

float gyro_x_radps, gyro_y_radps, gyro_z_radps;
Eigen::Vector3f gyro_radps;

float mag_x_ut, mag_y_ut, mag_z_ut;
Eigen::Vector3f mag_ut;

float die_temp_c;

void imu_setup()
{
  // Initialize I2C using Wire library
  Wire.begin(IMU_SDA_PIN, IMU_SCL_PIN);
  Wire.setClock(400000);

  // Configure IMU object
  imu.Config(&Wire, bfs::Mpu9250::I2C_ADDR_PRIM);

  // Initialize IMU 
  if (!imu.Begin()) {
    Serial.println("Error initializing communication with IMU");
    while(1) {}
  }
  // Set the IMU sample rate divider
  if (!imu.ConfigSrd(19)) {
    Serial.println("Error configuring SRD");
    while(1) {}
  }
}

void imu_update()
{
  // Read IMU
  if (imu.Read()) {
    // Update variables
    accel_mps2 = imu.accel_mps2();
    accel_x_mps2 = imu.accel_x_mps2();
    accel_y_mps2 = imu.accel_y_mps2();
    accel_z_mps2 = imu.accel_z_mps2();

    gyro_radps = imu.gyro_radps();
    gyro_x_radps = imu.gyro_x_radps();
    gyro_y_radps = imu.gyro_y_radps();
    gyro_z_radps = imu.gyro_z_radps();

    mag_ut = imu.mag_ut();
    mag_x_ut = imu.mag_x_ut();
    mag_y_ut = imu.mag_y_ut();
    mag_z_ut = imu.mag_z_ut();
    
    die_temp_c = imu.die_temp_c();
  }
}

#endif

