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

#include "config.h"
#include "util.h"
#include "node.h"
#include "drivers/imu.h"

rcl_publisher_t imu_publisher;  
rcl_timer_t imu_timer;

sensor_msgs__msg__Imu imu_msg;

float roll, pitch, yaw;

void imu_publish(rcl_timer_t * timer, int64_t last_call_time) {  
  update_time_header(&imu_msg.header.stamp);

  Eigen::Vector3f ypr_rad = bfs::TiltCompass(accel_mps2, mag_ut);
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

void imu_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
    "imu"
  ));
  Serial.println("Created rclc ROS Publisher /imu");

  // init timer
  RCCHECK(rclc_timer_init_default(&imu_timer, &support, RCL_MS_TO_NS(IMU_PUBLISHER_TIMER_INTERVAL), imu_publish));
  Serial.println("Created rclc timer for /imu");

  // add timer to executor
  RCCHECK(rclc_executor_add_timer(&executor, &imu_timer));
  Serial.println("Add rclc timer for /imu to executor");

  // set frame_id
  imu_msg.header.frame_id = micro_ros_string_utilities_init("imu_link");
}
