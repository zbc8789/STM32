#ifndef __USUALLY_H
#define __USUALLY_H

//ͷ�ļ�����
#include "stm32f10x.h"
#include "hardware.h"


//LED���Ŷ���
#define LED1 PGout(14)	   	//�����϶�Ӧ��LD1

//LED���Ŷ���
#define LED1 PDout(13)	   	//����LED��� D5
#define LED2 PGout(14)	   	//����LED��� D2

#define PD9 PDout(9)	   	//����LED��� D9
#define PB12 PBout(12)	   	//����LED��� B12


#define PDIN0 PDin(0)	   	//����LED��� D0
#define PDIN1 PDin(1)	   	//����LED��� D1
#define PDIN2 PDin(2)	   	//����LED��� D2
#define PDIN3 PDin(3)	   	//����LED��� D3
#define PDIN4 PDin(4)	   	//����LED��� D4
#define PDIN5 PDin(5)	   	//����LED��� D5
#define PDIN6 PDin(6)	   	//����LED��� D6

#endif
