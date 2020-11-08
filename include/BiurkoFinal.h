// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _BiurkoFinal_H_
#define _BiurkoFinal_H_

#include "Arduino.h"
#include "smartimpulsator.h"
#include "timer.h"
//add your includes for the project BiurkoFinal here


//end of add your includes here




//add your function definitions for the project BiurkoFinal here

class Biurko{
public:
	Biurko(): m_pulse(m_light_state.max_bound, 3, 12, false), m_light_state({LightState::Mode::high, m_light_state.max_bound, 0, 255}) {}


	struct LightState {
		enum class Mode {high, low, middle, high_no_limit, low_no_limit};
		Mode mode;
		uint8_t intensity;
		uint8_t min_bound;
		uint8_t max_bound;
	};


	void On(uint16_t power);

	void Off();
	void turn_off(int why);

	void turn_on(int why);


	uint32_t Get_waiter_time(){return waiter.how_much_to_wait();}
	bool Is_waiter_still_ticking(){return waiter.is_timer_still_ticking();}
	uint32_t Get_auto_off_time(){return auto_off.how_much_to_wait();}
	bool Is_auto_off_still_ticking(){return auto_off.is_timer_still_ticking();}
	uint32_t Get_motion_off_time(){return motion_off.how_much_to_wait();}
	bool Is_motion_off_still_ticking(){return motion_off.is_timer_still_ticking();}
//	const LightState& get_state() {return m_light_state;}

	void updates();

	void setups();
	uint8_t Get_light_intensity(){return m_light_state.intensity;}
private:
	void motion_callback();
	void debug(){
		Serial.println("update");
		debug_tm.set_timer(1000, [this](){this->debug();}, false);
	}
	void auto_light_make();


	void click_callback();

	void hold_callback();

	void waiting();

	void motion_short();

	long int tim=20;
	long int auto_off_time = 300000;
	long int short_time = 30000;



	const uint8_t motion_max_uses = 3;
	int act_motion_detect = 0;

	mikTimer auto_off;
	mikTimer motion_off;
	mikTimer waiter;

	mikTimer debug_tm;
	Guzik sensor;
	LightState m_light_state;
	SmartImpulsator m_pulse;
	Guzik ON_OFF;
};

//Do not add code below this line
#endif /* _BiurkoFinal_H_ */
