#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <micro_ros_arduino.h>
#include <ESP32Servo.h> 

#include "config.h"

int motor_pwm_min = 1250;
int motor_pwm_max = 1750;
// int motor_pwm_min = 500;
// int motor_pwm_max = 2500;

int motor_pwm;
int motor_pwm_left;
int motor_pwm_right;

// create servo objects to control ESCs
Servo motor_left;  
Servo motor_right;  

// Possible PWM GPIO pins on the ESP32: 0(used by on-board button),2,4,5(used by on-board LED),12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42 
int motor_pin_left = 18;      // GPIO pin used to connect the servo control (digital out)
int motor_pin_right = 19;      // GPIO pin used to connect the servo control (digital out)

void setup_motor_control()
{
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

  motor_left.setPeriodHertz(50);// Standard 50hz servo
  motor_left.attach(motor_pin_left, motor_pwm_min, motor_pwm_max);   // attaches the servo 

  motor_right.setPeriodHertz(50);// Standard 50hz servo
  motor_right.attach(motor_pin_right, motor_pwm_min, motor_pwm_max);   // attaches the servo
}

void set_motors_from_twist(double speed, double rotation)
{
  if(speed > 1)
    speed = 1;
    
  if(speed < -1)
    speed = -1;

#ifdef INVERSE_MOTORS
  speed = -speed;
#endif

  double left = speed;
  double right = speed;

  if(rotation != 0.00){
    if(speed != 0){
      if(rotation > 0.00){
        left = speed;
        right = speed * rotation;
      }else if(rotation < 0.00){
        left = speed * -rotation;
        right = speed;
      }
      //left = speed * rotation;
      //right = speed * -rotation;
    }else{
      left = -rotation;
      right = rotation;
    }
  }   

  motor_pwm_left = (left * ((motor_pwm_max - motor_pwm_min) / 2)) + motor_pwm_min + ((motor_pwm_max - motor_pwm_min) / 2);
  motor_pwm_right = (right * ((motor_pwm_max - motor_pwm_min) / 2)) + motor_pwm_min + ((motor_pwm_max - motor_pwm_min) / 2);

  // Update motor pwm
  motor_left.write(motor_pwm_left);
  motor_right.write(motor_pwm_right);
}

void set_motors_proportionaly(double speed){
  motor_pwm = (speed * ((motor_pwm_max - motor_pwm_min) / 2)) + motor_pwm_min + ((motor_pwm_max - motor_pwm_min) / 2);
  motor_pwm_left = motor_pwm;
  motor_pwm_right = motor_pwm;
}
#endif