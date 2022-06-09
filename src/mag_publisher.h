#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/magnetic_field.h>

#include "util.h"
#include "node.h"
#include "imu.h"

rcl_publisher_t mag_publisher;  

sensor_msgs__msg__MagneticField mag_msg;

void mag_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &mag_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, MagneticField),
    "magnetometer"
  ));
  Serial.println("Created rclc ROS Publisher /magnetometer");
}

void mag_publish() {
    mag_msg.header.frame_id = micro_ros_string_utilities_init("base_link");
    mag_msg.header.stamp.sec = millis()/1000;

    mag_msg.magnetic_field.x = mag_ut.x() * 0.000001;
    mag_msg.magnetic_field.y = mag_ut.y() * 0.000001;
    mag_msg.magnetic_field.z = mag_ut.z() * 0.000001;

    RCSOFTCHECK(rcl_publish(&mag_publisher, &mag_msg, NULL));

    // Serial.println("Magnetic field sent");
}