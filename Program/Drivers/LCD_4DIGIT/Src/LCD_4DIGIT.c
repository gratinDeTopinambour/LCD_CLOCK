/*
 * LCD_3DIGIT.c
 *
 *  Created on: 31 juil. 2026
 *      Author: valentin
 */

#include "LCD_4DIGIT.h"


static uint16_t font_index(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0' + 1);
    else if (c >= 'A' && c <= 'Z')
        return (c - 'A' + 11);
    else if (c >= 'a' && c <= 'z')
        return (c - 'a'+ 11);
    else if (c == '*')
            return 37;
    else
        return 0; // space / unknown
}

void LCD_WriteDigit(const char* str, uint8_t ponctuation)
{
	uint32_t RAM_reg0 = 0;
	uint32_t RAM_reg1 = 0;

	uint8_t car1 = 0;
	uint8_t car2 = 0;
	uint8_t car3 = 0;
	uint8_t car4 = 0;

	HAL_LCD_Clear(&hlcd);

	car1 = font7seg[font_index(*str)];
	str++;
	car2 = font7seg[font_index(*str)];
	str++;
	car3 = font7seg[font_index(*str)];
	str++;
	car4 = font7seg[font_index(*str)];
	str++;

	RAM_reg0 = (((car1 >> 6) & 1) << 7) |
			   (((car1 >> 5) & 1) << 25) |
			   (((car1 >> 4) & 1) << 26) |
			   (((car1 >> 3) & 1) << 27) |
			   (((car1 >> 1) & 1) << 17) |
			   (((car2 >> 7) & 1) << 21) |
			   (((car2 >> 6) & 1) << 16) |
			   (((car2 >> 5) & 1) << 13) |
			   (((car2 >> 4) & 1) << 14) |
			   (((car2 >> 3) & 1) << 15) |
			   (((car2 >> 2) & 1) << 9) |
			   (((car2 >> 1) & 1) << 8) |
			   (((car3 >> 6) & 1) << 0) |
			   (((car3 >> 5) & 1) << 6) |
			   (((car3 >> 4) & 1) << 10) |
			   (((car3 >> 3) & 1) << 11) |
			   (((car3 >> 2) & 1) << 20) |
			   (((car3 >> 1) & 1) << 19) |
			   (((car4 >> 6) & 1) << 3) |
			   (((car4 >> 5) & 1) << 4) |
			   (((car4 >> 4) & 1) << 22) |
			   (((car4 >> 3) & 1) << 23) |
			   (((car4 >> 2) & 1) << 2) |
			   (((car4 >> 1) & 1) << 1) |
			   (((ponctuation >> 3) & 1) << 18) | //COL
			   (((ponctuation >> 2) & 1) << 24) | //DIP1
			   (((ponctuation >> 1) & 1) << 12) | //DIP2
			   (((ponctuation >> 0) & 1) << 5); //DIP3

	HAL_LCD_Write(&hlcd, 0, 0xFFFFFFFF, RAM_reg0);

	RAM_reg1 = (((car3 >> 7) & 1) << 10) | //seg42
			   (((car4 >> 7) & 1) << 11) |
			   (((car1 >> 2) & 1) << 16) | //seg48
			   (((car1 >> 7) & 1) << 17);

	HAL_LCD_Write(&hlcd, 1, 0xFFFFFFFF, RAM_reg1);

	HAL_LCD_UpdateDisplayRequest(&hlcd);

}
