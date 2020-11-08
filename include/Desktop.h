// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Desktop_H_
#define _Desktop_H_
//#define DEBUG
#include "Arduino.h"
#include "smartimpulsator.h"
#include "timer.h"
//add your includes for the project Desktop here

class Biurko{
public:
	using Light_Callback = FunctionObject<void(uint32_t pin, uint32_t power)>;
	Biurko(Light_Callback analogWrited ): m_pulse(m_light_state.max_bound, 3, 12, false), m_light_state({LightState::Mode::high, m_light_state.max_bound, 0, 255}) {
		m_analogWrit = analogWrited;
	}

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
	void auto_light_make();


	void motion_callback();
	void click_callback();

	void hold_callback();
	void setups();
	uint8_t Get_light_intensity(){return m_light_state.intensity;}
private:
	Light_Callback m_analogWrit;
	void debug(){
		Serial.println("update");
		debug_tm.set_timer(1000, [this](){this->debug();}, false);
	}


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

	SmartImpulsator m_pulse;
	mikTimer debug_tm;

	LightState m_light_state;
};
//Do not add code below this line
#endif /* _Desktop_H_ */
