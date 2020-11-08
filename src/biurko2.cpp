// Do not remove the include below
#include "biurko2.h"
#include "impulsator.h"


//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(115200);
	set_impulsator(on_impulsator_rotate, 0);
	update_debug(0);
}

// The loop function is called in an endless loop
void loop()
{
	update_impulsator();
	update_light();
}
