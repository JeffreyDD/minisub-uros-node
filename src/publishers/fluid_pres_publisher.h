#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/fluid_pressure.h>

#include "config.h"
#include "util.h"
#include "node.h"

#include "drivers/fluid_pres_sensor.h"

rcl_publisher_t fluid_pres_publisher;  
rcl_timer_t fluid_pres_timer;

sensor_msgs__msg__FluidPressure fluid_pres_msg;

void fluid_pres_publish(rcl_timer_t * timer, int64_t last_call_time) {
    update_time_header(&fluid_pres_msg.header.stamp);

    fluid_pres_update();

    fluid_pres_msg.fluid_pressure = fluid_pres_pascals;

    RCSOFTCHECK(rcl_publish(&fluid_pres_publisher, &fluid_pres_msg, NULL));
    // Serial.println("FluidPressure sent");
}

void fluid_pres_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &fluid_pres_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, FluidPressure),
    "fluid_pressure"
  ));
  Serial.println("Created rclc ROS Publisher /fluid_pressure");

  // init timer
  RCCHECK(rclc_timer_init_default(&fluid_pres_timer, &support, RCL_MS_TO_NS(FLUID_PRES_PUBLISHER_TIMER_INTERVAL), fluid_pres_publish));
  Serial.println("Created rclc timer for /fluid_pressure");

  // add timer to executor
  RCCHECK(rclc_executor_add_timer(&executor, &fluid_pres_timer));
  Serial.println("Add rclc timer for /fluid_pressure to executor");

  // set frame_id on msg  
  fluid_pres_msg.header.frame_id = micro_ros_string_utilities_init("fluid_pres_sens_link");
}
