#include "main.h"

Biurko biurko;

void setup(){
	Serial.begin(9600);
	Serial.print("cus");
	biurko.setups();
}

void loop(){
	startup();
	biurko.updates();
}
