#ifndef __SIM900A_H__
#define __SIM900A_H__

#include "stm32f10x.h"
/*************	���غ�������	**************/
void CLR_Buf2(void);
u8 Find(char *a);
void Second_AT_Command(char *b,char *a,u8 wait_time);
void Set_ATE0(void);
void Connect_Server(void);
void Rec_Server_Data(void);
void Wait_CREG(void);
void Send_OK(void);


#endif


