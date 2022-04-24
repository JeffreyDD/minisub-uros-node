#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

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
    "topic_name"));
  Serial.println("Created rclc ROS Publisher");
}

void imu_publish() {
    imu_update();
    
    msg.linear_acceleration.x = aX;
    msg.linear_acceleration.y = aY;
    msg.linear_acceleration.z = aZ;

    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));

    Serial.println("Orientation sent");
}