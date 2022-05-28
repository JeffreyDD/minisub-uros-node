#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>

#include "util.h"
#include "node.h"
#include "imu_publisher.h"
#include "imu_publisher_raw.h"
#include "motor_control_twist_subscriber.h"
#include "motor_control.h"
#include "power_publisher.h"

#include "config.h"

void parameter_changed_callback(Parameter * param)
{
#ifdef MOTOR_CONTROL_ENABLED
  twist_subscription_parameter_handler(param);
#endif
}

void setup() {
  Serial.begin(115200);
  Serial.println("Setup");

  set_microros_wifi_transports(WIFI_SSID, WIFI_PASS, ROS_AGENT_IP, ROS_AGENT_PORT);
  Serial.println("Set microros transport to wifi");
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  

  delay(2000);

  // Setup node
  node_setup(NODE_NAME);
  Serial.println("Setup node done");

  // Add parameter server to node
  RCCHECK(rclc_executor_add_parameter_server(&executor, &param_server, parameter_changed_callback));
  printf("Parameter server setup done.\n");

#if defined(IMU_PUBLISHER_ENABLED) || defined(RAW_IMU_PUBLISHER_ENABLED)
  // Setup IMU
  imu_setup();
#endif

#ifdef IMU_PUBLISHER_ENABLED 
  // Setup IMU publisher
  imu_publisher_setup();
  
  Serial.println("Setup IMU Publisher done");
#endif

#ifdef RAW_IMU_PUBLISHER_ENABLED 
  // Setup IMU publisher
  raw_imu_publisher_setup();
  
  Serial.println("Setup Raw IMU Publisher done");
#endif

#ifdef MOTOR_CONTROL_ENABLED
  // Setup motor control (PWM pins)
  setup_motor_control();
  Serial.println("Setup motor control done");

  // Setup Twist subscriber
  twist_subscription_setup();
  Serial.println("Setup twist subscription done");
#endif

#ifdef POWER_PUBLISHER_ENABLED
  // Setup INA226 power meter board
  power_meter_setup();

  // Setup battery state publisher
  power_publisher_setup();
#endif

}

void loop() {
  // Serial.println("Looping");

#if defined(IMU_PUBLISHER_ENABLED) || defined(RAW_IMU_PUBLISHER_ENABLED)
  // Add a delay to ensure we're not segfaulting every 50 seconds?
  delay(100);
  
  // Gather IMU Data
  imu_update();
#endif

#ifdef IMU_PUBLISHER_ENABLED
  // Publish IMU Data
  imu_publish();
#endif

#ifdef IMU_PUBLISHER_ENABLED
  // Publish IMU Data
  imu_raw_publish();
#endif

#ifdef MOTOR_CONTROL_ENABLED
  // Spin executor to ensure twist callback is called when data is pushed to the topic
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  
  Serial.print(motor_pwm_left);
  Serial.print(",");
  Serial.println(motor_pwm_right);
#endif

#ifdef POWER_PUBLISHER_ENABLED
  power_publish();
#endif
  
}
