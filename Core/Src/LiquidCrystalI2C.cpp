/*
 * LiquidCrystalI2C.cpp
 *
 *  Created on: Oct 9, 2022
 *      Author: Alexandr
 */

#include "LiquidCrystalI2C.h"

LiquidCrystal_I2C::LiquidCrystal_I2C(I2C_HandleTypeDef hi2c, uint8_t addr,
		uint8_t rows, uint8_t cols) :
		_hi2c(hi2c), _addr(addr), _rows(rows), _cols(cols), _backlightval(
		LCD_NOBACKLIGHT) {

}

HAL_StatusTypeDef LiquidCrystal_I2C::expanderWrite(uint8_t data,
		uint8_t flags) {

	HAL_StatusTypeDef res;
	do {
		res = HAL_I2C_IsDeviceReady(&_hi2c, _addr << 1, 1, HAL_MAX_DELAY);
	} while (res != HAL_OK);

	uint8_t up = data & 0xF0;
	uint8_t lo = (data << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up | flags | _backlightval | PIN_EN;
	data_arr[1] = up | flags | _backlightval;
	data_arr[2] = lo | flags | _backlightval | PIN_EN;
	data_arr[3] = lo | flags | _backlightval;

	res = HAL_I2C_Master_Transmit(&_hi2c, _addr << 1, data_arr,
			sizeof(data_arr), HAL_MAX_DELAY);
	HAL_Delay(LCD_DELAY_MS);
	return res;
}

void LiquidCrystal_I2C::sendCommand(uint8_t cmd) {

	expanderWrite(cmd, 0);
}

void LiquidCrystal_I2C::sendData(uint8_t data) {

	expanderWrite(data, PIN_RS);
}

/********** high level commands, for the user! */
void LiquidCrystal_I2C::clear() {

	sendCommand(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
}

void LiquidCrystal_I2C::noBacklight() {

	_backlightval = LCD_NOBACKLIGHT;
	sendCommand(0);
}
void LiquidCrystal_I2C::backlight() {

	_backlightval = LCD_BACKLIGHT;
	sendCommand(0);
}

void LiquidCrystal_I2C::init() {

	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	 begin(_cols, _rows);

}
void LiquidCrystal_I2C::begin(uint8_t cols, uint8_t rows, uint8_t charsize) {

	if (rows > 1) {
			_displayfunction |= LCD_2LINE;
		}
		_numlines = rows;

		// for some 1 line displays you can select a 10 pixel high font
		if ((charsize != 0) && (rows == 1)) {
			_displayfunction |= LCD_5x10DOTS;
		}

		// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
		// according to datasheet, we need at least 40ms after power rises above 2.7V
		// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
		//HAL_Delay(Delay)(50);

		// Now we pull both RS and R/W low to begin commands
		sendCommand(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
		//HAL_Delay(1000);

	  	//put the LCD into 4 bit mode
		// this is according to the hitachi HD44780 datasheet
		// figure 24, pg 46

		  // we start in 8bit mode, try to set 4 bit mode
	   sendCommand(0x03 << 4);
	   //HAL_Delay(LCD_DELAY_MS);

	   // second try
	   sendCommand(0x03 << 4);
	   ///HAL_Delay(LCD_DELAY_MS);

	   // third go!
	   sendCommand(0x03 << 4);
	   //HAL_Delay(LCD_DELAY_MS);

	   // finally, set to 4-bit interface
	   sendCommand(0x02 << 4);


		// set # lines, font size, etc.
		sendCommand(LCD_FUNCTIONSET | _displayfunction);

		// turn the display on with no cursor or blinking default
		_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
		display();

		// clear it off
		clear();

		// Initialize to default text direction (for roman languages)
		_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

		// set the entry mode
		sendCommand(LCD_ENTRYMODESET | _displaymode);

		home();

}

void LiquidCrystal_I2C::home(){

	sendCommand(LCD_RETURNHOME);  // set cursor position to zero
}

void LiquidCrystal_I2C::display() {
	_displaycontrol |= LCD_DISPLAYON;
	sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}


void LiquidCrystal_I2C::setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
	sendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
void LiquidCrystal_I2C::printstr(const char* str){

	while(*str) {
	        sendData((uint8_t)(*str));
	        str++;
	    }
}
