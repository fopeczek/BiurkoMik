#include "lcd.h"
#include "Arduino.h"
#include "menu.h"
#include "blink.h"

FlatRawMenu menu;
static DebugLCD mylcd;

LiquidCrystal_I2C lcd(0x3F, 20, 4);

auto draw_act_state1(){
	String out= "istt: ";
	if (biurko.Is_motion_off_still_ticking()){
		out = out + "yes " + String(biurko.Get_motion_off_time()/1000) + " to end";
	} else{
		out += "no";
	}
	return out;
}

String draw_act_state2(){
	String out = "iltt: ";
	if (biurko.Is_auto_off_still_ticking()){
		out += "yes " + String(biurko.Get_auto_off_time()/1000) + " to end";
	} else{
		out += "no";
	}
	return out;
}

String draw_act_state3() {
	String out = "idtt: ";
	if (biurko.Is_waiter_still_ticking()){
		out += "yes " + String(biurko.Get_waiter_time()/1000) + " to end";
	} else{
		out += "no";
	}
	return out;
}

String draw_act_state4() {
	String out = "light is: " + String(biurko.Get_light_intensity());
	return out;
}

void debug_menu_item(int row, bool selected){
	lcd.setCursor(0,row);
	if (selected){
		lcd.print("> ");
	}
	lcd.print("Debug  ");
}

void off_menu_item(int row, bool selected){
	lcd.setCursor(0,row);
	if (selected){
		lcd.print("> ");
	}
	lcd.print("Off  ");
}

void on_menu_item(int row, bool selected){
	lcd.setCursor(0,row);
	if (selected){
		lcd.print("> ");
	}
	lcd.print("On  ");
}

void debug_menu_item_callback(){
	lcd.clear();
	menu_enabled = false;
	draw_act_state();
	debug_enabled = true;
}

void startup(){
	lcd.begin();
	lcd.backlight();
	lcd.clear();
	menu.init_screen(4, 3);
	menu.add_item(debug_menu_item, debug_menu_item_callback, 0);
	menu.add_item(off_menu_item, light_min, 1);
	menu.add_item(on_menu_item, light_max, 2);
	mylcd.set_draw_row_fn(0, draw_act_state1);
	mylcd.set_draw_row_fn(1, draw_act_state2);
	mylcd.set_draw_row_fn(2, draw_act_state3);
	mylcd.set_draw_row_fn(3, draw_act_state4);
	menu.draw();
}

void select(){
	menu.select();
}

void down(){
	menu.move_selection_down();
}

void menuDraw(){
	lcd.clear();
	menu.draw();
}

void draw_act_state() {
	mylcd.update();
}


//void draw_act_state(){
//	lcd.home();
//	lcd.clear();
//	lcd.print("istt: ");
//	if (motion_off.is_timer_still_ticking()){
//		lcd.print("yes ");
//		lcd.print(motion_off.how_much_to_wait()/1000);
//		lcd.print("to end");
//	} else{
//		lcd.print("no");
//	}
//
//
//	lcd.setCursor(0,1);
//	lcd.print("iltt: ");
//	if (auto_off.is_timer_still_ticking()){
//		lcd.print("yes ");
//		lcd.print(auto_off.how_much_to_wait()/1000);
//		lcd.print("to end");
//	} else{
//		lcd.print("no");
//	}
//
//
//	lcd.setCursor(0,2);
//	lcd.print("encoder is: ");
//	lcd.print(pulse.GetValue());
//	lcd.print(" light is");
//	lcd.print(act_light);
//
//
//	lcd.setCursor(0,3);
//	lcd.print("motion is: ");
//	lcd.print(act_motion_detect);
//	lcd.print(" no limit is: ");
//	if (no_limit){
//		lcd.print("on");
//	} else{
//		lcd.print("off");
//	}
//}


void DebugLCD::update_row(uint8_t row_nr) {
	if (row_nr >= ROW_COUNT)
		return;

	String row_content = m_callbacks[row_nr]();

	if (m_content[row_nr] == row_content)
		return;

	m_content[row_nr] = row_content;

	if (row_content.length() > COL_COUNT) {
		row_content.remove(COL_COUNT, row_content.length() - COL_COUNT);
	} else if (row_content.length() < COL_COUNT) {
		row_content += String("                                       ").substring(0, COL_COUNT - row_content.length());
	}
	lcd.setCursor(0, row_nr);
	lcd.print(row_content);
}


void DebugLCD::update() {
	for(uint8_t i=0;i<ROW_COUNT;i++)
		update_row(i);
}
