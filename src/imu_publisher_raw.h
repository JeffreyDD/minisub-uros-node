#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/type_utilities.h>

#include <std_msgs/msg/float32_multi_array.h>

#include "util.h"
#include "node.h"
#include "imu.h"

rcl_publisher_t raw_imu_publisher;  

std_msgs__msg__Float32MultiArray raw_imu_msg;

static micro_ros_utilities_memory_conf_t conf = {0};

void raw_imu_publisher_setup(){
  // Allocate msg memory
  bool success = micro_ros_utilities_create_message_memory(
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    &raw_imu_msg,
    conf
  );

  // Create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &raw_imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    "imu_raw"
  ));

  Serial.println("Created rclc ROS Publisher /imu_raw");
}

void imu_raw_publish() {
  // Set msg size to number of elements in array
  raw_imu_msg.data.size = 10;

  // Populate array with raw data from sensor
  raw_imu_msg.data.data[0] = accel_x_mps2;
  raw_imu_msg.data.data[1] = accel_y_mps2;
  raw_imu_msg.data.data[2] = accel_z_mps2;
  raw_imu_msg.data.data[3] = gyro_x_radps;
  raw_imu_msg.data.data[4] = gyro_y_radps;
  raw_imu_msg.data.data[5] = gyro_z_radps;
  raw_imu_msg.data.data[6] = mag_x_ut;
  raw_imu_msg.data.data[7] = mag_y_ut;
  raw_imu_msg.data.data[8] = mag_z_ut ;
  raw_imu_msg.data.data[9] = die_temp_c;

  // Publish raw imu data
  RCSOFTCHECK(rcl_publish(&raw_imu_publisher, &raw_imu_msg, NULL));

  // Serial.println("Raw IMU data sent");
}