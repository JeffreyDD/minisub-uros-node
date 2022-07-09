#ifndef POWER_PUBLISHER_H_
#define POWER_PUBLISHER_H_

#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

//#include <micro_ros_utilities/string_utilities.h>
#include <micro_ros_utilities/type_utilities.h>

#include <sensor_msgs/msg/battery_state.h>

#include "config.h"
#include "util.h"
#include "node.h"

#include "drivers/ina226.h"

#include "config.h"

rcl_publisher_t power_publisher;  
rcl_timer_t power_timer;

sensor_msgs__msg__BatteryState power_msg;

static micro_ros_utilities_memory_conf_t power_mem_conf = {0};

void power_publish(rcl_timer_t * timer, int64_t last_call_time) {
    ina226_update();

    update_time_header(&power_msg.header.stamp);

    power_msg.voltage = power_voltage;
    power_msg.current = power_current;

    RCSOFTCHECK(rcl_publish(&power_publisher, &power_msg, NULL));
}

void power_publisher_setup(){
  bool success = micro_ros_utilities_create_message_memory(
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, BatteryState),
    &power_msg,
    power_mem_conf
  );
  // Serial.println("Power memory allocated");

  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &power_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, BatteryState),
    TOPIC_POWER_NAME
  ));
  Serial.println("Created rclc ROS Publisher /battery");

  // init timer
  RCCHECK(rclc_timer_init_default(&power_timer, &support, RCL_MS_TO_NS(POWER_PUBLISHER_TIMER_INTERVAL), power_publish));
  Serial.println("Created rclc timer for /battery");

  // add timer to executor
  RCCHECK(rclc_executor_add_timer(&executor, &power_timer));
  Serial.println("Add rclc timer for /battery to executor");
}

#endif