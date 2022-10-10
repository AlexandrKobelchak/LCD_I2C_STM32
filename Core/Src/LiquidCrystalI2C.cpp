/*
 * LiquidCrystalI2C.cpp
 *
 *  Created on: Oct 9, 2022
 *      Author: Alexandr
 */

#include "LiquidCrystalI2C.h"

LiquidCrystal_I2C::LiquidCrystal_I2C(I2C_HandleTypeDef hi2c, uint8_t addr, uint8_t rows, uint8_t cols)
	:_hi2c(hi2c), _addr(addr), _rows(rows), _cols(cols), _backlightval(LCD_BACKLIGHT){



}



HAL_StatusTypeDef LiquidCrystal_I2C::expanderWrite(uint8_t data, uint8_t flags) {

	HAL_StatusTypeDef res;
    do {
        res = HAL_I2C_IsDeviceReady(&_hi2c, _addr << 1, 1, HAL_MAX_DELAY);
    }while (res != HAL_OK);

    uint8_t up = data & 0xF0;
    uint8_t lo = (data << 4) & 0xF0;

    uint8_t data_arr[4];
    data_arr[0] = up|flags|_backlightval|PIN_EN;
    data_arr[1] = up|flags|_backlightval;
    data_arr[2] = lo|flags|_backlightval|PIN_EN;
    data_arr[3] = lo|flags|_backlightval;

    res = HAL_I2C_Master_Transmit(&_hi2c, _addr << 1, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
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
void LiquidCrystal_I2C::clear(){
	sendCommand(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	HAL_Delay(LCD_DELAY_MS);
}
