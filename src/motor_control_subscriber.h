#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>

#include "util.h"
#include "motor_control.h"

rcl_subscription_t twist_subscriber;
geometry_msgs__msg__Twist twist_msg;
rclc_executor_t twist_executor;

//twist message cb
void twist_subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist * twist_msg = (const geometry_msgs__msg__Twist *)msgin;
  
  // if velocity in x direction is 0 turn off LED, if 1 turn on LED
  digitalWrite(LED_PIN, (twist_msg->linear.x == 0) ? LOW : HIGH);
  
  // Process message
  Serial.print("Twist received: Linear X:");
  Serial.print(twist_msg->linear.x);
  Serial.print(", Angular Z:");
  Serial.println(twist_msg->angular.z);

  double speed = twist_msg->linear.x;
  double rotation = twist_msg->angular.z;

  set_motors_from_twist(speed, rotation);
}

void twist_subscription_setup(){
    // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &twist_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"));

  // create executor
  RCCHECK(rclc_executor_init(&twist_executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&twist_executor, &twist_subscriber, &twist_msg, &twist_subscription_callback, ON_NEW_DATA));
}


