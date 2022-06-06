#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/imu.h>

#include "eigen.h"
#include "units.h"
#include "navigation.h"

#include "util.h"
#include "node.h"
#include "imu.h"

rcl_publisher_t imu_publisher;  

sensor_msgs__msg__Imu imu_msg;

void imu_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
    "imu"
  ));
  Serial.println("Created rclc ROS Publisher /imu");
}

float roll, pitch, yaw;
int secs, nanos;

void imu_publish() {  
  secs = millis()/1000;
  nanos = millis()-(secs*1000)*1000;

  imu_msg.header.frame_id = micro_ros_string_utilities_init("base_link");
  imu_msg.header.stamp.sec = secs;
  imu_msg.header.stamp.nanosec = nanos;

  // imu_msg.orientation_covariance = {0.0025, 0, 0, 0, 0.0025, 0, 0, 0, 0.0025};
  // imu_msg.angular_velocity_covariance = {0.0025, 0, 0, 0, 0.0025, 0, 0, 0, 0.0025};
  // imu_msg.linear_acceleration_covariance = {0.0025, 0, 0, 0, 0.0025, 0, 0, 0, 0.0025};

  //roll = (float)atan2(acc_gyro.raw.acc_y, acc_gyro.raw.acc_z);
  // float accel_x_g = bfs::convacc(accel_x_mps2, bfs::LinAccUnit::MPS2, bfs::LinAccUnit::G);
  // float accel_y_g = bfs::convacc(accel_y_mps2, bfs::LinAccUnit::MPS2, bfs::LinAccUnit::G);
  // float accel_z_g = bfs::convacc(accel_z_mps2, bfs::LinAccUnit::MPS2, bfs::LinAccUnit::G);
  
  // // Calculate roll
  // roll = (float)atan2(accel_y_g, accel_z_g);

  // // Calculate pitch
  // if (accel_y_g * sin(roll) + accel_z_g * cos(roll) == 0){
  //   if (accel_x_g>0){
  //     pitch = PI / 2;
  //   } else{
  //     pitch = -PI / 2;
  //   }
  // }else{
  //   pitch = (float)atan(-accel_x_g / (accel_y_g * sin(roll) + accel_z_g * cos(roll)));
  // }

  // // Calculate yaw
  // yaw = (float)atan2(mag_x_ut * sin(roll) - mag_y_ut * cos(roll), mag_x_ut * cos(pitch) + mag_y_ut * sin(pitch) * sin(roll) + mag_z_ut * sin(pitch) * cos(roll));
  
  Eigen::Vector3f ypr_rad = bfs::TiltCompass(accel_mps2, mag_ut);
  //printf("%f,%f,%f\n",- ypr_rad.x(), ypr_rad.y(), -ypr_rad.z());
  Eigen::Quaternionf quat = bfs::angle2quat(-ypr_rad.x(), ypr_rad.y(), -ypr_rad.z()); // Build quat on X over Y over Z, how IMU's normally work

  // Set orientation
  imu_msg.orientation.x = quat.x();
  imu_msg.orientation.y = quat.y();
  imu_msg.orientation.z = quat.z();
  imu_msg.orientation.w = quat.w();

  // Set linear accelaration
  imu_msg.linear_acceleration.x = accel_x_mps2;
  imu_msg.linear_acceleration.y = accel_y_mps2;
  imu_msg.linear_acceleration.z = accel_z_mps2;

  // Set angular velocity
  imu_msg.angular_velocity.x = gyro_x_radps;
  imu_msg.angular_velocity.y = gyro_y_radps;
  imu_msg.angular_velocity.z = gyro_z_radps;

  RCSOFTCHECK(rcl_publish(&imu_publisher, &imu_msg, NULL));

  // Serial.println("Orientation sent");
}