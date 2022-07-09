#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/magnetic_field.h>

#include "config.h"
#include "util.h"
#include "node.h"
#include "drivers/imu.h"

rcl_publisher_t mag_publisher;  
rcl_timer_t mag_timer;

sensor_msgs__msg__MagneticField mag_msg;

void mag_publish(rcl_timer_t * timer, int64_t last_call_time) {
    update_time_header(&mag_msg.header.stamp);

    mag_msg.magnetic_field.x = mag_ut.x() * 0.000001;
    mag_msg.magnetic_field.y = mag_ut.y() * 0.000001;
    mag_msg.magnetic_field.z = mag_ut.z() * 0.000001;

    RCSOFTCHECK(rcl_publish(&mag_publisher, &mag_msg, NULL));

    // Serial.println("Magnetic field sent");
}

void mag_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &mag_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, MagneticField),
    "magnetometer"
  ));
  Serial.println("Created rclc ROS Publisher /magnetometer");

  // init timer
  RCCHECK(rclc_timer_init_default(&mag_timer, &support, RCL_MS_TO_NS(MAG_PUBLISHER_TIMER_INTERVAL), mag_publish));
  Serial.println("Created rclc timer for /magnetometer");

  // add timer to executor
  RCCHECK(rclc_executor_add_timer(&executor, &mag_timer));
  Serial.println("Add rclc timer for /magnetometer to executor");

  // set frame_id on msg  
  mag_msg.header.frame_id = micro_ros_string_utilities_init("imu_link");
}
