//This file handles all stuff related to the impulsator

#include "biurko2.h"
#include "impulsator.h"


int16_t light_intensity;

const int16_t max_impulsator = 255;

void on_impulsator_rotate(void*, int8_t value) {
	if(light_intensity<value)
		light_intensity=0;
	else if(light_intensity>max_impulsator)
		light_intensity=max_impulsator;
	else
		light_intensity+=value;
}
