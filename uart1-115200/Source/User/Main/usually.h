#ifndef __USUALLY_H
#define __USUALLY_H

//头文件调用
#include "stm32f10x.h"
#include "hardware.h"


//LED引脚定义
#define LED1 PGout(14)	   	//板子上对应于LD1

//LED引脚定义
#define LED1 PDout(13)	   	//板上LED编号 D5
#define LED2 PGout(14)	   	//板上LED编号 D2

#define PD9 PDout(9)	   	//板上LED编号 D9
#define PB12 PBout(12)	   	//板上LED编号 B12


#define PDIN0 PDin(0)	   	//板上LED编号 D0
#define PDIN1 PDin(1)	   	//板上LED编号 D1
#define PDIN2 PDin(2)	   	//板上LED编号 D2
#define PDIN3 PDin(3)	   	//板上LED编号 D3
#define PDIN4 PDin(4)	   	//板上LED编号 D4
#define PDIN5 PDin(5)	   	//板上LED编号 D5
#define PDIN6 PDin(6)	   	//板上LED编号 D6

#endif
