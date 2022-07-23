#ifndef FLUID_SENSOR_H
#define FLUID_SENSOR_H

#include <micro_ros_arduino.h>

#define PRES_SENS_PIN 34
#define PRES_SENS_MIN_PASCALS 0
#define PRES_SENS_MAX_PASCALS 1200000
// #define PRES_SENS_MAX_PASCALS 792000 // Adjusted for 3.3v

float fluid_pres_pascals;

void fluid_pres_update(){
    int fluid_pres_reading = analogRead(PRES_SENS_PIN);
    float fluid_pres_rel = (float)fluid_pres_reading / (float)4096;
    fluid_pres_pascals = (fluid_pres_rel * PRES_SENS_MAX_PASCALS - PRES_SENS_MIN_PASCALS) + PRES_SENS_MIN_PASCALS;

    // Serial.print(fluid_pres_reading);
    // Serial.print(",");
    // Serial.print(fluid_pres_rel);
    // Serial.print(",");
    // Serial.println(fluid_pres_pascals);
}

#endif