#include "Arduino.h"

float get_temp(); //returns current temperature - either from clock or from potentiometer

float temp_from_time(float time);
float intensity_from_time(float time);

void update_light(); //displays current light. Called once 5 minutes, or after impulsator event
void update_debug(void *);

extern int16_t light_intensity;

void on_impulsator_rotate(void*, int8_t value);
