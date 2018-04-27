#ifndef __USART_H
#define __USART_H

//头文件包含
#include "stdio.h"	 
#include <string.h>

//函数声明
void USART1_Send_Byte(u16 dat);
uint8_t USART1_Receive_Byte(void);
void Init_Usart(void);
void Usart_Configuration(uint32_t BaudRate); 

#define USART1_REC_MAXLEN 200	//最大接收数据长度

void USART1_Init_Config(u32 bound);
//void UART1_SendString(char* s);
void USART2_Init_Config(u32 bound);
void UART2_SendString(char* s);
//void UART1_SendLR(void);
void UART2_SendLR(void);

//串口1发送回车换行
//#define UART1_SendLR() UART1_SendString("\r\n")

//											 USART_SendData(USART1,0X0D);\
//											 
//											 USART_SendData(USART1,0X0A)
//串口2发送回车换行
#define UART2_SendLR() UART2_SendString("\r\n")
//											 USART_SendData(USART2,0X0D);\
//											 USART_SendData(USART2,0X0A)


#endif
