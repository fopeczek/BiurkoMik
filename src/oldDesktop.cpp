// Do not remove the include below
#include "Desktop.h"
#include "piny.h"
#include "click.h"
#include "timer.h"
#include "smartimpulsator.h"
#include "impulsator.h"

void Biurko::motion_callback(){
	Serial.println("motion");
	if (m_light_state.mode != LightState::Mode::high_no_limit and m_light_state.mode != LightState::Mode::low_no_limit){
		if (m_light_state.intensity != 0 and act_motion_detect == 0){
//				blink_motion(1, PIN_LED_MOTION);
			turn_on(2);
			motion_off.stop_timer();

		}else if ((m_light_state.intensity == 0 and not waiter.is_timer_still_ticking()) or act_motion_detect != 0) {
			act_motion_detect += 1;
//				blink_motion(act_motion_detect, PIN_LED_MOTION);
			turn_on(3);

			Serial.print("act motion detect: ");
			Serial.println(act_motion_detect);
		}
		if (act_motion_detect == motion_max_uses){

			motion_off.stop_timer();
			turn_on(2);
		}
	}
}

void Biurko::auto_light_make(){
	Serial.println(m_pulse.GetValue());
//		blink_encoder(1, PIN_LED_IMPULSATOR);
	digitalWrite(PIN_LED_MOTION_LONG, HIGH);
	if (m_light_state.mode != LightState::Mode::high_no_limit and m_light_state.mode != LightState::Mode::low_no_limit){
		if (m_pulse.GetValue() == m_light_state.min_bound) {
			m_light_state.mode = LightState::Mode::low;
			Off();
		} else if (m_pulse.GetValue() == m_light_state.max_bound) {
			m_light_state.mode = LightState::Mode::high;
			On(m_light_state.max_bound);
		} else{
			m_light_state.mode = LightState::Mode::middle;
			On(m_pulse.GetValue());
			auto_off.set_timer(auto_off_time, [this](){this->turn_off(2);}, false);
		}
	}else {
		digitalWrite(PIN_LED_NO_LIMIT, LOW);
		auto_off.set_timer(auto_off_time, [this](){this->turn_off(2);}, false);
		m_light_state.mode = LightState::Mode::middle;
	}
}

void Biurko::waiting(){ //motion activation
	if (m_light_state.mode != LightState::Mode::high_no_limit and m_light_state.mode != LightState::Mode::low_no_limit){
		Serial.println("motion activate");
	}else {
		waiter.set_timer(short_time, [this](){this->waiting();}, false);
	}
}

void Biurko::motion_short(){ //sort time ends
	auto_off.stop_timer();
	motion_off.stop_timer();

	act_motion_detect = 0;

	Off();
	Serial.println("short time");
}

void Biurko::On(uint16_t power){
	if (power > 0) {
		analogWrite(PIN_LED_WHITE, power);
		m_light_state.intensity=power;
		m_pulse.setValue(m_light_state.intensity);
		Serial.println(m_light_state.intensity);
		waiter.stop_timer();
	}else {
		Serial.println("Please use value above 0, automatically turning off light");
		Off();
	}
}

void Biurko::Off(){
	analogWrite(PIN_LED_WHITE, m_light_state.min_bound);
	m_light_state.intensity=m_light_state.min_bound;
	m_pulse.setValue(m_light_state.intensity);
	waiter.set_timer(short_time, [this](){this->waiting();}, false);
	Serial.println("motion deactivate by off");
	digitalWrite(PIN_LED_MOTION_LONG, LOW);
	auto_off.stop_timer();
	motion_off.stop_timer();
}

void Biurko::turn_off(int why) {
 //was 1 - guzik; 2 - time_out; 3 - impulsator; 4 - empty; 5 - no limit;
	if (why == 1){//1 - guzik
		Serial.println("zgasiłem światło");
		Serial.println("było włączone");

		Off();
		m_light_state.mode = LightState::Mode::low;
		act_motion_detect = 0;

		waiter.set_timer(short_time, [this](){this->waiting();}, false);
		Serial.println("motion deactivate");
		digitalWrite(PIN_LED_NO_LIMIT, LOW);
		digitalWrite(PIN_LED_MOTION_LONG, LOW);
		auto_off.stop_timer();
	} else if (why == 2){//2 - time_out
		Serial.println("zgasiłem światło");
		Serial.println("przez time out");

		Off();
		m_light_state.mode = LightState::Mode::low;

		digitalWrite(PIN_LED_MOTION_LONG, LOW);
		digitalWrite(PIN_LED_NO_LIMIT, LOW);
		auto_off.stop_timer();
	} else if (why == 3){ //3 - impulsator
		if ((m_light_state.intensity < 127 and not m_light_state.intensity==0) or m_light_state.intensity == m_light_state.max_bound){
			Serial.println("zgasiłem światło");
			Serial.println("było na mikpulsie");

			Off();
			m_light_state.mode = LightState::Mode::low;

			waiter.set_timer(short_time, [this](){this->waiting();}, false);
			Serial.println("motion deactivate");
			digitalWrite(PIN_LED_MOTION_LONG, LOW);
			digitalWrite(PIN_LED_NO_LIMIT, LOW);
			auto_off.stop_timer();
		}
	} else if (why == 5){//5 - no limit
		Serial.println("zgasiłem światło");
		Serial.println("na bez limitu");

		Off();
		m_light_state.mode = LightState::Mode::low_no_limit;

		waiter.set_timer(short_time, [this](){this->waiting();}, false);
		Serial.println("motion deactivate for no limit");
		digitalWrite(PIN_LED_MOTION_LONG, LOW);
		auto_off.stop_timer();
		motion_off.stop_timer();
	}
}


void Biurko::turn_on(int why){ //was - 1 guzik; 2 - motion(for long); 3 - motion(for short); 4 - impulsator; 5 - no time limit;
	if (why == 1){
		Serial.println("zapaliłem światło");
		Serial.println("było wyłączone");

		On(m_light_state.max_bound);
		m_light_state.mode = LightState::Mode::high;

		auto_off.set_timer(auto_off_time, [this](){this->turn_off(2);}, false);
		digitalWrite(PIN_LED_MOTION_LONG, HIGH);
		digitalWrite(PIN_LED_NO_LIMIT, LOW);
	} else if (why == 3){
		Serial.println("zapaliłem światło");
		Serial.println("na krótko");

		On(m_light_state.max_bound);
		m_light_state.mode = LightState::Mode::high;


		digitalWrite(PIN_LED_NO_LIMIT, LOW);
		digitalWrite(PIN_LED_MOTION_LONG, HIGH);
		motion_off.set_timer(short_time, [this](){this->motion_short();}, false);
		if (act_motion_detect == motion_max_uses) {
			act_motion_detect = 0;
			turn_on(2);
			motion_off.stop_timer();
		}
	} else if (why == 2){
		Serial.println("zapaliłem światło");
		Serial.println("na długo/przedłużyłem czas");

		On(m_light_state.intensity);
		m_light_state.mode = LightState::Mode::high;

		digitalWrite(PIN_LED_NO_LIMIT, LOW);
		digitalWrite(PIN_LED_MOTION_LONG, HIGH);
		auto_off.set_timer(auto_off_time, [this](){this->turn_off(2);}, false);
		motion_off.stop_timer();
	} else if (why == 4){
		if ((m_light_state.intensity >= 127 or m_light_state.intensity==0) and not m_light_state.intensity==m_light_state.max_bound){
			Serial.println("zapaliłem światło");
			Serial.println("było mikpulsie");

			On(m_light_state.max_bound);
			m_light_state.mode = LightState::Mode::high;

			digitalWrite(PIN_LED_NO_LIMIT, LOW);
			digitalWrite(PIN_LED_MOTION_LONG, HIGH);
			auto_off.set_timer(auto_off_time, [this](){this->turn_off(2);}, false);
		}
	} else if (why == 5){
		Serial.println("zapaliłem światło");
		Serial.println("bez limitu czasu");

		On(m_light_state.max_bound);
		m_light_state.mode = LightState::Mode::high_no_limit;
		digitalWrite(PIN_LED_MOTION_LONG, LOW);
		auto_off.stop_timer();
		motion_off.stop_timer();
		waiter.stop_timer();
	}
}

void Biurko::hold_callback(){ //hold
	Serial.println("hold");
//		blink_no_limit(2, PIN_LED_NO_LIMIT);
	if (m_light_state.mode == LightState::Mode::low){
		turn_on(5);
	}else if (m_light_state.mode == LightState::Mode::high){
		turn_off(5);
	}else if (m_light_state.mode == LightState::Mode::middle){
		turn_off(5);
	}else if (m_light_state.mode == LightState::Mode::high_no_limit){
		turn_off(5);
	}else if (m_light_state.mode == LightState::Mode::low_no_limit){
		turn_on(5);
	}
}

void Biurko::click_callback() { //click
	Serial.println("click");
//		blink_click(1, PIN_LED_CLICK);
	if (m_light_state.mode == LightState::Mode::high) { // if it is on
		turn_off(1);
	}else if (m_light_state.mode == LightState::Mode::low) { // if not
		turn_on(1);
	}else if (m_light_state.mode == LightState::Mode::middle) {
		turn_on(4);
		turn_off(3);
	}else if (m_light_state.mode == LightState::Mode::high_no_limit) {
		turn_on(1);
	}else if (m_light_state.mode == LightState::Mode::low_no_limit) {
		turn_off(1);
	}
}


void Biurko::setups(){
	Serial.begin(9600);
	Serial.println("start");
	Serial.println(m_light_state.intensity);
	Serial.println(m_pulse.GetValue());
	ON_OFF.setupUsingAnalogPin(PIN_CHANGE_COLOR_BUTTON);
	ON_OFF.setupClickHandler([this](){this->click_callback();});
	ON_OFF.setupHoldHandler([this](){this->hold_callback();});
	sensor.setupUsingAnalogPin(PIN_MOTION_SENSOR);
	sensor.setupClickHandler([this](){this->motion_callback();});

	m_pulse.setOnChange([this](){this->auto_light_make();});

	pinMode(PIN_LED_WHITE, OUTPUT);
	pinMode(PIN_LED_MOTION, OUTPUT);
	pinMode(PIN_LED_IMPULSATOR, OUTPUT);
	pinMode(PIN_LED_CLICK, OUTPUT);
	pinMode(PIN_LED_NO_LIMIT, OUTPUT);
	pinMode(PIN_LED_MOTION_LONG, OUTPUT);
	pinMode(9, OUTPUT);
	analogWrite(PIN_LED_WHITE, m_light_state.max_bound);


	m_light_state.intensity = m_light_state.max_bound;
	m_light_state.mode = LightState::Mode::high;
	m_pulse.setValue(m_light_state.max_bound, m_light_state.max_bound);
	turn_on(2);

	//debug_tm.set_timer(1000, debug, false);
}

void Biurko::updates(){
	ON_OFF.update();
	m_pulse.update();
	auto_off.update();
	sensor.update();
	motion_off.update();
	waiter.update();
	debug_tm.update();
}
