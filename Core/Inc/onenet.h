#ifndef _ONENET_H_
#define _ONENET_H_


extern int set_hour,set_minute;


_Bool OneNET_RegisterDevice(void);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNET_Subscribe(void);

void OneNet_RevPro(unsigned char *cmd);

#endif
