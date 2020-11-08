#ifndef LCD_H_
#define LCD_H_

#include "function_objects.h"
#include "LiquidCrystal_I2C.h"

#define ROW_COUNT 4
#define COL_COUNT 20

extern LiquidCrystal_I2C lcd;

class DebugLCD {
	using draw_row_fn = FunctionObject<String(void)>;
public:
	DebugLCD() {
		for(uint8_t i=0; i<ROW_COUNT; i++)
			m_content[i] = "";
	}
	void set_draw_row_fn(uint8_t row, draw_row_fn callback) {
		if (row < ROW_COUNT)
			m_callbacks[row] = callback;
	}
	void update();
private:
	void update_row(uint8_t row);
	String m_content[ROW_COUNT];
	draw_row_fn m_callbacks[ROW_COUNT];

};

void menuDraw();
void select();
void down();
void startup();
void draw_act_state();


#endif /* LCD_H_ */
