#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/imu.h>

#include "units.h"

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

  // Set orientation
  imu_msg.orientation.x = quat_x;
  imu_msg.orientation.y = quat_y;
  imu_msg.orientation.z = quat_z;
  imu_msg.orientation.w = quat_w;

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