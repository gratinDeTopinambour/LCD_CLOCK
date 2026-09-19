/*
 * LCD_3DIGIT.h
 *
 *  Created on: 31 juil. 2026
 *      Author: valentin
 */

#ifndef LCD_4DIGIT_INC_LCD_4DIGIT_H_
#define LCD_4DIGIT_INC_LCD_4DIGIT_H_

#include <stdint.h>
#include "main.h"

#include "7segment_font.h"

extern LCD_HandleTypeDef hlcd;

void LCD_WriteDigit(const char* str, uint8_t ponctuation);

#endif /* LCD_4DIGIT_INC_LCD_4DIGIT_H_ */
