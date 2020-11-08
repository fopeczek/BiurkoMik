#include "blink.h"
#include "lcd.h"
#include "piny.h"
#include "Arduino.h"

Guzik ON_OFF;
Guzik sensor;
Guzik menu_bt;

bool menu_enabled = true;
bool debug_enabled = false;

void analogWrit(uint32_t pin, uint32_t power){
	analogWrite(pin, power);
}
Biurko biurko(analogWrit);

void motion(){
	biurko.motion_callback();
}

void menu_click(){
	if (menu_enabled){
		down();
	}
}

void menu_hold(){
	if (menu_enabled){
		select();
	}else {
		menu_enabled = true;
		debug_enabled = false;
		menuDraw();
	}
}

void hold(){ //hold
	biurko.hold_callback();
}

void click() { //click
	biurko.click_callback();
}


void setup(){
	Serial.begin(9600);
	Serial.print("cus");
	Serial.flush();
	ON_OFF.setupClickHandler(click);
	ON_OFF.setupUsingAnalogPin(PIN_CHANGE_COLOR_BUTTON);
	ON_OFF.setupClickHandler(hold);
	menu_bt.setupClickHandler(menu_click);
	menu_bt.setupUsingAnalogPin(PIN_MENU_BUTTON);
	menu_bt.setupHoldHandler(menu_hold);
//	ON_OFF.setupClickHandler([this](){this->click_callback();});
	sensor.setupUsingAnalogPin(PIN_MOTION_SENSOR);
	sensor.setupClickHandler(motion);


	pinMode(PIN_LED_WHITE, OUTPUT);
	pinMode(PIN_LED_MOTION, OUTPUT);
	pinMode(PIN_LED_IMPULSATOR, OUTPUT);
	pinMode(PIN_LED_CLICK, OUTPUT);
	pinMode(PIN_LED_NO_LIMIT, OUTPUT);
	pinMode(PIN_LED_MOTION_LONG, OUTPUT);
	pinMode(PIN_LED_MOTION_LONG, OUTPUT);
	pinMode(9, OUTPUT);
	biurko.setups();
	startup();
}

void loop(){
	if (debug_enabled){
		draw_act_state();
	}
	menu_bt.update();
	biurko.updates();
	ON_OFF.update();
	sensor.update();
}

void light_max() {
	biurko.On(255);
}

void light_min() {
	biurko.Off();
}
