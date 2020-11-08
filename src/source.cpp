#include "biurko2.h"
#include "piny.h"
#include "libRGBW.h"
#include "Streaming.h"
#include "timer2.h"

PerceptualConverter light_model;
WhiteConverter RGBW_converter;


float debug_intensity;
float debug_przedintensity;
float debug_max_intensity;
RGB debug_light;
RGBW debug_leds;
perceptualRGB debug_plight;

mikTimer timer;



float temp_from_time(float time) {
	constexpr auto Rtmp_max = 7000;
	constexpr auto Rtmp_min = 2000;
	constexpr auto Rgodz_min = 6;
	constexpr auto Rgodz_max = 23;
	return max(Rtmp_min, ((Rtmp_max-Rtmp_min)*sin((time-Rgodz_min)/(Rgodz_max-Rgodz_min)*PI)+Rtmp_min));
}

float intensity_from_time(float time) {
	constexpr auto Itmp_max = 100;
	constexpr auto Itmp_min = 6;
	constexpr auto Igodz_min = 4;
	constexpr auto Igodz_max = 25;
	if(time<Igodz_min) {
		time+=24;
	}
	return max(0., ((Itmp_max-Itmp_min)*sin((time-Igodz_min)/(Igodz_max-Igodz_min)*PI)+Itmp_min));
}


float get_time(){
	//todo - być może jakieś 2 tryby: tryb potencjometru lub tryb czasu.
	return analogRead(PIN_POTENTIOMETER)* 0.024414062;//0..100.0
}

//input x is 0..1. Returns value from 0..10.
float gamma(float x) {
	constexpr float a = 3.61495;
	//a:  FindRoot[10 == -1/a + Exp[a]/a, {a, 10}]
	return (-1 + exp(a*x))/a;
//	return (255 - ( par_gamma_B + par_gamma_A * exp((70-actpos)*par_gamma_a)));
}

void shine(float light, int where) {
	if(light<0)
		light=0;
	else if (light>255)
		light=255;
	analogWrite(where, light);
}

void update_debug(void *) {
	Serial<<"time: "<<get_time()<<", temp: "<<temp_from_time(get_time())<<"\n";
	Serial<<"pRGB("<<debug_plight.pR<<", "<<debug_plight.pG<<", "<<debug_plight.pB<<") ";
	Serial<<"RGB("<<debug_light.R<<", "<<debug_light.G<<", "<<debug_light.B<<")\n";
	Serial<<"power: "<<debug_intensity<<" max_power: "<<debug_max_intensity<<" ";
	Serial<<"RGBW("<<debug_leds.R<<", "<<debug_leds.G<<", "<<debug_leds.B<<", "<<debug_leds.W<<" )\n";


//	Serial<<"time: "<<get_time()<<", temp: "<<temp_from_time(get_time())<<", ";
//	Serial<<"intensity przed gamma: "<<debug_przedintensity<<", ";
//	Serial<<"intensity po gamma: "<<debug_intensity<<", max_intensity: "<<debug_max_intensity<<", ";
	timer.set_timer(2000, update_debug, 0);
}

void update_light() {
//	delay (500);
//	debug_show();
	float time = get_time();
	float temp = temp_from_time(time);
	debug_przedintensity=intensity_from_time(time)*0.01;
	float intensity = gamma(intensity_from_time(time)*0.01)*0.45;


	debug_intensity=gamma(debug_przedintensity)*0.45;
	perceptualRGB pcolor = light_model.Temperature2RGB(temp, intensity);
	RGB color = light_model.convertToRGB(pcolor);
	debug_max_intensity=RGBW_converter.calculateMaxIntensity(color);
	debug_plight=pcolor;
	debug_light=color;
	RGBW light = RGBW_converter.getRGBW(color);
	debug_leds = light;
	shine(light.W, PIN_LED_WHITE);
	shine(light.R, PIN_LED_RED);
	shine(light.G, PIN_LED_GREEN);
	shine(light.B, PIN_LED_BLUE);
	timer.update();
}

