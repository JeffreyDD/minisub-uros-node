#ifndef MOTOR_CONTROL_TWIST_SUBSCRIBER_H_
#define MOTOR_CONTROL_TWIST_SUBSCRIBER_H_

#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>

#include "util.h"
#include "node.h"
#include "motor_control.h"

rcl_subscription_t twist_subscriber;
geometry_msgs__msg__Twist twist_msg;

//twist message cb
void twist_subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist * twist_msg = (const geometry_msgs__msg__Twist *)msgin;
  
  // if velocity in x direction is 0 turn off LED, if 1 turn on LED
  digitalWrite(LED_PIN, (twist_msg->linear.x == 0) ? LOW : HIGH);
  
  // Process message
  // Serial.print("Twist received: Linear X:");
  // Serial.print(twist_msg->linear.x);
  // Serial.print(", Angular Z:");
  // Serial.println(twist_msg->angular.z);

  double speed = twist_msg->linear.x;
  double rotation = twist_msg->angular.z;

  set_motors_from_twist(speed, rotation);
}

void twist_subscription_setup(){
  // Add parameters
  RCCHECK(rclc_add_parameter(&param_server, "pwm_min", RCLC_PARAMETER_INT));
  RCCHECK(rclc_add_parameter(&param_server, "pwm_max", RCLC_PARAMETER_INT));

  // Set parameter defaults
  rclc_parameter_set_int(&param_server, "pwm_min", motor_pwm_min);
  rclc_parameter_set_int(&param_server, "pwm_max", motor_pwm_max);

  // FIXME: Set parameter descriptions and constraints

  // Create subscription
  RCCHECK(rclc_subscription_init_default(
    &twist_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"));

  // Add subscription to node executor
  RCCHECK(rclc_executor_add_subscription(&executor, &twist_subscriber, &twist_msg, &twist_subscription_callback, ON_NEW_DATA));
}

void twist_subscription_parameter_handler(Parameter * param){
  printf("twist_subscription_parameter_handler got changed parameter %s\n", param->name.data);
  
  if(param->name.data =="pwm_min"){
    rclc_parameter_get_int(&param_server, "pwm_min", &motor_pwm_min);
    printf("parameter pwm_min updated to %i\n", motor_pwm_min);
  }
  if(param->name.data =="pwm_max"){
    rclc_parameter_get_int(&param_server, "pwm_max", &motor_pwm_max);
    printf("parameter pwm_max updated to %i\n", motor_pwm_max);
  }
}

#endif
