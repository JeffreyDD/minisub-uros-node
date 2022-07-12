#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/type_utilities.h>

#include <std_msgs/msg/float32_multi_array.h>

#include "config.h"
#include "util.h"
#include "node.h"
#include "drivers/imu.h"

rcl_publisher_t raw_imu_publisher;  
rcl_timer_t raw_imu_timer;

std_msgs__msg__Float32MultiArray raw_imu_msg;

static micro_ros_utilities_memory_conf_t raw_imu_mem_conf = {0};

void imu_raw_publish(rcl_timer_t * timer, int64_t last_call_time) {
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
}

void raw_imu_publisher_setup(){
  // Allocate msg memory
  micro_ros_utilities_create_message_memory(
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    &raw_imu_msg,
    raw_imu_mem_conf
  );

  // Create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &raw_imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    "imu_raw"
  ));
  Serial.println("Created rclc ROS Publisher /imu_raw");

  // init timer
  RCCHECK(rclc_timer_init_default(&raw_imu_timer, &support, RCL_MS_TO_NS(RAW_IMU_PUBLISHER_TIMER_INTERVAL), imu_raw_publish));
  Serial.println("Created rclc timer for /imu_raw");

  // add timer to executor
  RCCHECK(rclc_executor_add_timer(&executor, &raw_imu_timer));
  Serial.println("Add rclc timer for /imu_raw to executor");
}