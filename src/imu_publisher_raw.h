#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

//#include <micro_ros_utilities/string_utilities.h>
#include <micro_ros_utilities/type_utilities.h>

//#include <sensor_msgs/msg/imu.h>
#include <std_msgs/msg/float32_multi_array.h>

#include "util.h"
#include "node.h"
#include "imu.h"

rcl_publisher_t raw_imu_publisher;  

std_msgs__msg__Float32MultiArray raw_imu_msg;

static micro_ros_utilities_memory_conf_t conf = {0};

void raw_imu_publisher_setup(){
  bool success = micro_ros_utilities_create_message_memory(
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    &raw_imu_msg,
    conf
  );

  // create publisher
  RCCHECK(rclc_publisher_init_best_effort(
    &raw_imu_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32MultiArray),
    "imu_raw"
  ));

  Serial.println("Created rclc ROS Publisher /imu_raw");
}

void imu_raw_publish() {
    // Serial.print("IMU Updated: ");
    // Serial.print(aX);
    // Serial.print(",");
    // Serial.print(aY);
    // Serial.print(",");
    // Serial.println(aZ);

    // rosidl_runtime_c__String frame_id = "base_link";
    
    // msg.header.frame_id = micro_ros_string_utilities_init("base_link");
    // msg.header.stamp.sec = millis()/1000;

    //msg.data.capacity = 11;
    raw_imu_msg.data.size = 11;

    raw_imu_msg.data.data[0] = aX;
    raw_imu_msg.data.data[1] = aY;
    raw_imu_msg.data.data[2] = aZ;
    raw_imu_msg.data.data[3] = aSqrt;
    raw_imu_msg.data.data[4] = gX;
    raw_imu_msg.data.data[5] = gY;
    raw_imu_msg.data.data[6] = gZ;
    raw_imu_msg.data.data[7] = mDirection;
    raw_imu_msg.data.data[8] = mX;
    raw_imu_msg.data.data[9] = mY;
    raw_imu_msg.data.data[10] = mZ;

    RCSOFTCHECK(rcl_publish(&raw_imu_publisher, &raw_imu_msg, NULL));

    // Serial.println("Raw IMU data sent");
}