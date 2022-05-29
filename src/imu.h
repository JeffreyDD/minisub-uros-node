#ifndef IMU_H_
#define IMU_H_

#include <SparkFunMPU9250-DMP.h> // Include SparkFun MPU-9250-DMP library
MPU9250_DMP imu;


float accel_x_mps2, accel_y_mps2, accel_z_mps2;
float gyro_x_radps, gyro_y_radps, gyro_z_radps;
float mag_x_ut, mag_y_ut, mag_z_ut;
float quat_x, quat_y, quat_z, quat_w;
float die_temp_c;

void imu_setup()
{
  // Enable compass sensor directly
  imu.setSensors(INV_XYZ_COMPASS); 

  // // Use setGyroFSR() and setAccelFSR() to configure the
  // // gyroscope and accelerometer full scale ranges.
  // // Gyro options are +/- 250, 500, 1000, or 2000 dps
  // imu.setGyroFSR(2000); // Set gyro to 2000 dps
  // // Accel options are +/- 2, 4, 8, or 16 g
  // imu.setAccelFSR(2); // Set accel to +/-2g

  // // setLPF() can be used to set the digital low-pass filter
  // // of the accelerometer and gyroscope.
  // // Can be any of the following: 188, 98, 42, 20, 10, 5
  // // (values are in Hz).
  // imu.setLPF(5); // Set LPF corner frequency to 5Hz

  // // The sample rate of the accel/gyro can be set using
  // // setSampleRate. Acceptable values range from 4Hz to 1kHz
  // imu.setSampleRate(10); // Set sample rate to 10Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate(10); // Set mag rate to 10Hz

  // Start IMU
  if (imu.begin() != INV_SUCCESS)
  {
      while (1)
      {
          printf("Error while initialising IMU");
      }
  }

  // Initialize the digital motion processor for accelerometer and gyro
  imu.dmpBegin(DMP_FEATURE_SEND_RAW_ACCEL | // Send accelerometer data
                DMP_FEATURE_GYRO_CAL       | // Calibrate the gyro data
                DMP_FEATURE_SEND_CAL_GYRO  | // Send calibrated gyro data
                DMP_FEATURE_6X_LP_QUAT     , // Calculate quat's with accel/gyro
                10);                         // Set update rate to 10Hz.
}

void imu_update()
{
  // Check for new data in the IMU's FIFO buffer
  if ( imu.fifoAvailable() > 0 ) 
  {
      // Use dmpUpdateFifo to update the ax, gx, qx, etc. values
      if ( imu.dmpUpdateFifo() == INV_SUCCESS )
      {
          // The following variables will have data from the top of the FIFO:
          // imu.ax, imu.ay, imu.az, -- Accelerometer
          // imu.gx, imu.gy, imu.gz -- calibrated gyroscope
          // and imu.qw, imu.qx, imu.qy, and imu.qz -- quaternions

          accel_x_mps2 = bfs::convacc(imu.calcAccel(imu.ax), bfs::LinAccUnit::G, bfs::LinAccUnit::MPS2);
          accel_y_mps2 = bfs::convacc(imu.calcAccel(imu.ay), bfs::LinAccUnit::G, bfs::LinAccUnit::MPS2);
          accel_z_mps2 = bfs::convacc(imu.calcAccel(imu.az), bfs::LinAccUnit::G, bfs::LinAccUnit::MPS2);

          gyro_x_radps = bfs::convangacc(imu.calcGyro(imu.gx), bfs::AngAccUnit::DEGPS2, bfs::AngAccUnit::RADPS2);
          gyro_y_radps = bfs::convangacc(imu.calcGyro(imu.gy), bfs::AngAccUnit::DEGPS2, bfs::AngAccUnit::RADPS2);
          gyro_z_radps = bfs::convangacc(imu.calcGyro(imu.gz), bfs::AngAccUnit::DEGPS2, bfs::AngAccUnit::RADPS2);

          quat_x = imu.calcQuat(imu.qx);
          quat_y = imu.calcQuat(imu.qy);
          quat_z = imu.calcQuat(imu.qz);
          quat_w = imu.calcQuat(imu.qw);
      }
  }

  imu.update(UPDATE_COMPASS | UPDATE_TEMP);

  mag_x_ut = imu.calcMag(imu.mx);
  mag_y_ut = imu.calcMag(imu.my);
  mag_z_ut = imu.calcMag(imu.mz);

  die_temp_c = imu.temperature;
}

#endif

