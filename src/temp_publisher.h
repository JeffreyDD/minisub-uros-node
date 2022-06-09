#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/temperature.h>

#include "util.h"
#include "node.h"
#include "imu.h"

rcl_publisher_t temp_publisher;  

sensor_msgs__msg__Temperature temp_msg;

void temp_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &temp_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Temperature),
    "temperature"
  ));
  Serial.println("Created rclc ROS Publisher /temperature");
}

void temp_publish() {
    temp_msg.header.frame_id = micro_ros_string_utilities_init("base_link");
    temp_msg.header.stamp.sec = millis()/1000;

    temp_msg.temperature = die_temp_c;

    RCSOFTCHECK(rcl_publish(&temp_publisher, &temp_msg, NULL));

    // Serial.println("Temperature sent");
}