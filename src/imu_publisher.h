#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/string_utilities.h>

#include <sensor_msgs/msg/imu.h>

#include "util.h"
#include "node.h"
#include "imu.h"

rcl_publisher_t publisher;  

sensor_msgs__msg__Imu msg;

void imu_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
    "imu"
  ));
  Serial.println("Created rclc ROS Publisher /imu");
}

void imu_publish() {
    imu_update();

    // Serial.print("IMU Updated: ");
    // Serial.print(aX);
    // Serial.print(",");
    // Serial.print(aY);
    // Serial.print(",");
    // Serial.println(aZ);

    // rosidl_runtime_c__String frame_id = "base_link";
    
    msg.header.frame_id = micro_ros_string_utilities_init("base_link");
    msg.header.stamp.sec = millis()/1000;

    msg.linear_acceleration.x = aX;
    msg.linear_acceleration.y = aY;
    msg.linear_acceleration.z = aZ;

    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));

    Serial.println("Orientation sent");
}