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

rcl_publisher_t imu_publisher;  

sensor_msgs__msg__Imu imu_msg;

void imu_publisher_setup(){
  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
    "imu"
  ));
  Serial.println("Created rclc ROS Publisher /imu");
}

void imu_publish() {
    // Serial.print("IMU Updated: ");
    // Serial.print(aX);
    // Serial.print(",");
    // Serial.print(aY);
    // Serial.print(",");
    // Serial.println(aZ);

    // rosidl_runtime_c__String frame_id = "base_link";

    double q[4];
    euler_to_quat(aX * 180, aY * 180, aZ * 180, q);
    
    imu_msg.header.frame_id = micro_ros_string_utilities_init("base_link");
    imu_msg.header.stamp.sec = millis()/1000;

    imu_msg.orientation.w = q[0];
    imu_msg.orientation.x = q[1];
    imu_msg.orientation.y = q[2];
    imu_msg.orientation.z = q[3];

    imu_msg.linear_acceleration.x = gX;
    imu_msg.linear_acceleration.y = gY;
    imu_msg.linear_acceleration.z = gZ;

    RCSOFTCHECK(rcl_publish(&imu_publisher, &imu_msg, NULL));

    // Serial.println("Orientation sent");
}