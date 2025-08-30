#ifndef _VOICE_H_
#define _VOICE_H_
#include "main.h"

#define Dispensing_medicine									"A8:02/00001*MP3"                //出药中，请稍等
#define Drug_release_complete					      "A8:02/00002*MP3"								 //出药完成，请取药
#define Welcome_voice												"A8:02/00003*MP3"								 //欢迎进入
#define Warning_voice												"A8:02/00004*MP3"							   //仓库药品数量为零
#define Alarm_clock_reminder								"A8:02/00005*MP3"								 //闹钟提醒
#define Inventory_reminder									"A8:02/00006*MP3"								 //仓库库存
#define Link_network_compete								"A8:02/00007*MP3"								 //链接网络完成
#define Link_network												"A8:02/00008*MP3"                //链接网络中
#define Doctor_prescription									"A8:02/00009*MP3"                //医生药方


#define slide_arrive_press_to_carport			"A8:02/00010*MP3"
#define slide_arrive_drive_to_slide				"A8:02/00011*MP3"
#define slide_arrive_press_to_slide				"A8:02/00012*MP3"
#define slide_arrive_ground_pay						"A8:02/00013*MP3"
#define welcome_to_come_again							"A8:02/00014*MP3"
#define no_more_carports									"A8:02/00015*MP3"
#define move_to_your_carports							"A8:02/00016*MP3"
#define leave_to_your_carports						"A8:02/00017*MP3"
#define detect_went_to_carports						"A8:02/00018*MP3"
#define detect_went_to_slide							"A8:02/00019*MP3"
#define please_next_one										"A8:02/00020*MP3"
#define pay_sucecess_leave								"A8:02/00021*MP3"
#define pay_8															"A8:02/00022*MP3"
#define pay_16														"A8:02/00023*MP3"
#define pay_24														"A8:02/00024*MP3"
#define carnumb_detect										"A8:02/00025*MP3"
#define louder														"AF:30"

void voice_uart_printf(char *fmt, ...);//重定向usart2
#endif

