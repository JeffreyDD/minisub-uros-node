#ifndef THRUST_SUBSCRIBER_H_
#define THRUST_SUBSCRIBER_H_

#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/float32_multi_array.h>

#include "util.h"
#include "node.h"
#include "config.h"

#include "drivers/motor_control.h"

rcl_subscription_t thrust_subscriber;
std_msgs__msg__Float32MultiArray thrust_msg;

static micro_ros_utilities_memory_conf_t thrust_mem_conf = {0};

void thrust_subscription_callback(const void *msgin) {
  const std_msgs__msg__Float32MultiArray * thrust_msg = (const std_msgs__msg__Float32MultiArray *)msgin;
  
  set_motors_proportionaly(thrust_msg->data.data[THRUSTER_INDEX]);
}

void thrust_subscription_setup(){
  // Allocate msg memory
  micro_ros_utilities_create_message_memory(
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    &thrust_msg,
    thrust_mem_conf
  );

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &thrust_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    "thrusters_power"));

  RCCHECK(rclc_executor_add_subscription(&executor, &thrust_subscriber, &thrust_msg, &thrust_subscription_callback, ON_NEW_DATA));
}

#endif
