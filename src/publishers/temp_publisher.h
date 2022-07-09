#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/temperature.h>

#include "config.h"
#include "util.h"
#include "node.h"

#include "drivers/imu.h"

rcl_publisher_t temp_publisher;  
rcl_timer_t temp_timer;

sensor_msgs__msg__Temperature temp_msg;

void temp_publish(rcl_timer_t * timer, int64_t last_call_time) {
    update_time_header(&temp_msg.header.stamp);

    temp_msg.temperature = die_temp_c;

    RCSOFTCHECK(rcl_publish(&temp_publisher, &temp_msg, NULL));
    // Serial.println("Temperature sent");
}

void temp_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &temp_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Temperature),
    "temperature"
  ));
  Serial.println("Created rclc ROS Publisher /temperature");

  // init timer
  RCCHECK(rclc_timer_init_default(&temp_timer, &support, RCL_MS_TO_NS(TEMP_PUBLISHER_TIMER_INTERVAL), temp_publish));
  Serial.println("Created rclc timer for /temperature");

  // add timer to executor
  RCCHECK(rclc_executor_add_timer(&executor, &temp_timer));
  Serial.println("Add rclc timer for /temperature to executor");

  // set frame_id on msg  
  temp_msg.header.frame_id = micro_ros_string_utilities_init("imu_link");
}
