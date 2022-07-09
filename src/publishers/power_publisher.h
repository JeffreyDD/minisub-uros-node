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

#include "util.h"
#include "node.h"

#include "drivers/ina226.h"

#include "config.h"

rcl_publisher_t power_publisher;  

sensor_msgs__msg__BatteryState power_msg;

static micro_ros_utilities_memory_conf_t power_mem_conf = {0};

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
}

// Very rudimentary timing mechanism
int lastPowerPub = 0;
int powerPubInterval = 500;

void power_publish() {
  if(lastPowerPub + powerPubInterval < millis()){
    
    ina226_update();
    
    power_msg.voltage = power_voltage;
    power_msg.current = power_current;

    RCSOFTCHECK(rcl_publish(&power_publisher, &power_msg, NULL));

    lastPowerPub = millis();
  }
}

#endif