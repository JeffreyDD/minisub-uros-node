#ifndef INA226_H_
#define INA226_H_

#include <GyverINA.h>

INA226 ina;

float power_voltage;
float power_current;

void ina226_setup(){
  if (ina.begin()) {
    Serial.println(F("INA226 connected!"));
  } else {
    Serial.println(F("INA226 not found!"));
    while (1);
  }

  // Serial.print(F("Calibration value: ")); 
  // Serial.println(ina.getCalibration());

  ina.setSampleTime(INA226_VBUS,   INA226_CONV_2116US);  
  ina.setSampleTime(INA226_VSHUNT, INA226_CONV_8244US); 
  ina.setAveraging(INA226_AVG_X4); 
}

void ina226_update(){
    power_voltage = ina.getVoltage();
    power_current = ina.getCurrent();
}

#endif