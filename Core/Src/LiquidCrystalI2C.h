/*
 * LiquidCrystalI2C.h
 *
 *  Created on: Oct 9, 2022
 *      Author: Alexandr
 */
#include "main.h"


#ifndef SRC_LIQUIDCRYSTALI2C_H_
#define SRC_LIQUIDCRYSTALI2C_H_
#define LCD_DELAY_MS 5

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define PIN_EN 0x04  // Enable bit
#define PIN_RW 0x02  // Read/Write bit
#define PIN_RS 0x01  // Register select bit

class LiquidCrystal_I2C {

private:

	I2C_HandleTypeDef _hi2c;
	uint8_t _addr;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _numlines;
	uint8_t _rows;
	uint8_t _cols;
	uint8_t _backlightval;

public:
	LiquidCrystal_I2C(I2C_HandleTypeDef hi2c, uint8_t addr, uint8_t rows, uint8_t cols);


	void init();
	void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
	void clear();
	void home();
	void noDisplay();
	void display();
	void noBlink();
	void blink();
	void noCursor();
	void cursor();
	void scrollDisplayLeft();
	void scrollDisplayRight();
	void printLeft();
	void printRight();
	void leftToRight();
	void rightToLeft();
	void shiftIncrement();
	void shiftDecrement();
	void noBacklight();
	void backlight();
	void autoscroll();
	void noAutoscroll();
	void createChar(uint8_t, uint8_t[]);
	void setCursor(uint8_t, uint8_t);
	size_t write(uint8_t);
	void command(uint8_t);

	////compatibility API function aliases
	void blink_on();					// alias for blink()
	void blink_off();       			// alias for noBlink()
	void cursor_on();      	 			// alias for cursor()
	void cursor_off();      			// alias for noCursor()
	void setBacklight(uint8_t new_val);	// alias for backlight() and nobacklight()
	void load_custom_character(uint8_t char_num, uint8_t *rows);// alias for createChar()
	void printstr(const char*);

private:

	void init_priv();
	void send(uint8_t, uint8_t);
	void write4bits(uint8_t);
	void pulseEnable(uint8_t);




	HAL_StatusTypeDef expanderWrite(uint8_t data, uint8_t flags);
	void sendCommand(uint8_t cmd);
	void sendData(uint8_t data);

};

#endif /* SRC_LIQUIDCRYSTALI2C_H_ */
