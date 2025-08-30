#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "main.h"

enum page_num{
	page_start = 1,
	page_main = 2,
	page_index = 3,
	page_1 = 4,
	page_2 = 5,
	page_3 = 6,
	page_4 = 7,
};

void Page_turn(uint8_t page_num);
void Send_message(void);
void Page_num6(void);
void Page_num7(void);
#endif





