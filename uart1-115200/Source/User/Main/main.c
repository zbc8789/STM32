//ͷ�ļ�����
#include "usually.h"
#include "usart.h"
#include "stm32f10x_exti.h"
//��������
const char menu[] =
   "\n\r"
   "+********************* STM32������  ********************+\n\r"	   //"\n"���ڳ����ն˵������ǻ���
   "|                                                       |\n\r"	   //"\r" :�ڳ����ն˵��������ö�
   "|                  USART �첽ͨ������                   |\n\r"
   "|                   www.scdbit.com                      |\n\r"
   "|                   �ɶ���һ�ؿƼ�                      |\n\r"
   "|                      2012.5.15                        |\n\r"
   "|                ����֧��QQ��406795003                  |\n\r"
   "+-------------------------------------------------------+\n\r";

//��������
void Init_LED(void);	
void Init_NVIC(void);
void Init_TI_KEY(void);
void Delay_Ms(uint16_t time);  
void Delay_Us(uint16_t time); 
int getshumaguannNum(int val);

int gLED1 = 0xff;
int gLED2 = 0xff;

int gLED1_irq = 0;
int gLED2_irq = 0;
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: main
** ��������: ���������
** �������������������ʾ�˴����շ����� �������ǲ����жϷ�ʽ
				�������������ϵ�����ֵ ���Գ����ն˽���ʮ����������������ֵ 
				���磺1��Ӧʮ������49��2��Ӧ��50��a��Ӧ��97������㶮������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int main(void)
{
	
	int num1 = 0, num2= 0;
	SystemInit();					//ϵͳʱ������
	Init_NVIC();					//�ж�������ע�ắ��
	Init_LED();						//����������������
	Init_TI_KEY();				//?????????
	Init_Usart();					//������������
	Usart_Configuration(115200);	//�������� ���ò�����Ϊ115200
	printf(menu);					//����ַ���
	printf("��һ�����㿪��STM32����,�����������ţ���\n\r");
	printf("����������ϵ������ַ������ڽ���ʮ���������������ַ�\n\r");		   
	while(1)													
	{ 
			if(gLED1_irq == 1)
			{
				gLED1_irq = 0;
				gLED1 = getnum();
			}
			
			if(gLED2_irq == 1){
				gLED2_irq = 0;
				gLED2 = getnum();
			}
			
			if(gLED1 != 0xff ){
				if(num1 > 20){
					//printf("[1]led1 = 0x%x \r\n",gLED1);
					printf("\r\n[1]LED1 = %d \r\n",getshumaguannNum(gLED1));
					num1 = 0;
				}
				num1 ++;
				gLED1 = 0xff;
				//gLED2 = 0xff;
			}
			if(gLED2 != 0xff){
				
				if(num2 > 20){
					//printf("[2]led2 = 0x%x \r\n" , gLED2);
					printf("\r\n[2]led2 = %d \r\n", getshumaguannNum(gLED2));
					num2 = 0;
				}
				num2 ++;
				//gLED1 = 0xff;
				gLED2 = 0xff;
			}
			
		
//		LED1=~LED1;	   				
//		Delay_Ms(200);				 //LED1��˸��ϵͳ��������
	}
}

int getshumaguannNum(int val){
	char num = 0xff;
	
	switch(val){
		
		case 0x40:
		num = 0;
		break;
		
		case 0x79:
		num = 1;
		break;
		
		case 0x24:
		num = 2;
		break;
		
		case 0x30:
		num = 3;
		break;
		
		case 0x19:
		num = 4;
		break;
		
		case 0x12:
		num = 5;
		break;
		
		case 0x02:
		num = 6;
		break;
		
		case 0x78:
		num = 7;
		break;
		
		case 0x0:
		num = 8;
		break;
		
		case 0x10:
		num = 9;
		break;
	
		default:
			break;
	}
//	printf(" num = %d val = 0x%x \r\n", num, val);
	return num;
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_LED
** ��������: LED IO��������
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE,ENABLE);	//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;										//����LED�˿ڹҽӵ�6��12��13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				   	//���ݲ�����ʼ��GPIOD�Ĵ���
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			 //����LED��� D5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	

	
	// D0 --D5  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			 // D0
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			 //D1
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			 //D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			 //D3
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			 //D4
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			 //D5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			 //D6
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ????: KEY_TI_Init
** ????: ?? ???? 
** ????:?
** ?   ?: Dream
** ?   ?: 2011?6?20?
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_TI_KEY(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;					//????EXTI?????

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		//??IO????,????????!!!

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);//????C?13??????	  ??!! ????INT2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0); //????E?0??????	  ??!! ????INT1
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource12); //????E?0??????	  ??!! ????INT1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource11); //????E?0??????	  ??!! ????INT1
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line13 | EXTI_Line12 | EXTI_Line11 ;//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//?????????
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//EXTI_Trigger_Falling;	//?????
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//?????
	EXTI_Init(&EXTI_InitStructure);							//????????????
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_NVIC
** ��������: ϵͳ�ж�����
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��5��14��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_NVIC(void)
{ 	
  	NVIC_InitTypeDef NVIC_InitStructure;			//����һ��NVIC������ṹ�����

	#ifdef  VECT_TAB_RAM  							//���������ַѡ��

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//��0x20000000��ַ��Ϊ���������ַ(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //��0x08000000��ַ��Ϊ���������ַ(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж��� Ϊ2 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//���ô���1Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 	//����ռ�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//���ø����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//ʹ�ܴ���1�ж�
	NVIC_Init(&NVIC_InitStructure);							  	//���ݲ�����ʼ���жϼĴ���
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//�趨�ж�ԴΪPC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�ж�ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//??????PE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//???????1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//?????0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//????
	NVIC_Init(&NVIC_InitStructure);	
	
	
}
int getnum(void){
//	char d1 = 0, d2 = 0, d3 =0,  d5 = 0, d6 = 0, d7 = 0;
	int data = 0;
//	char d4 = 0;
	
//	Delay_Ms(1);
	if(PDIN0 == 1){
		data += 0x01;
//		d1 = PDIN0;
	}
//	printf("data1=0x%x\r\n",data);
	if(PDIN1 == 1){
		data += 0x02;
//		d2 = PDIN1;
	}
//	printf("data2=0x%x\r\n",data);
	if(PDIN2 == 1){
		data |= 0x04;
//		d3 = PDIN2;
	}
//	printf("data3=0x%x\r\n",data);
	if(PDIN3 == 1){
		data += 0x8;
//		d4 = PDIN3;
	}
//	printf("data4=0x%x\r\n",data);
	if(PDIN4 == 1){
		data += 0x10;
//		d5 = PDIN4;
	}
//	printf("data5=0x%x\r\n",data);
	if(PDIN5 == 1){
		data += 0x20;
//		d6 = PDIN5;
	}
//	printf("data6=0x%x\r\n",data);
	if(PDIN6 == 1){
		data += 0x40;
//		d7 = PDIN6;
	}
//	if(data != 0x7f)
//		printf("data7=0x%x\r\n",data);
	if(data == 0x7f){
		data = 0xff;
	}
	return data ;
}

int getnum2(void){
//	char d1 = 0, d2 = 0, d3 =0,  d5 = 0, d6 = 0, d7 = 0;
	int data = 0;
//	char d4 = 0;
	
	Delay_Ms(10);
	if(PDIN0 == 1){
		data += 0x01;
//		d1 = PDIN0;
	}
//	printf("data1=0x%x\r\n",data);
	if(PDIN1 == 1){
		data += 0x02;
//		d2 = PDIN1;
	}
//	printf("data2=0x%x\r\n",data);
	if(PDIN2 == 1){
		data |= 0x04;
//		d3 = PDIN2;
	}
//	printf("data3=0x%x\r\n",data);
	if(PDIN3 == 1){
		data += 0x8;
//		d4 = PDIN3;
	}
//	printf("data4=0x%x\r\n",data);
	if(PDIN4== 1){
		data += 0x10;
//		d5 = PDIN4;
	}
//	printf("data5=0x%x\r\n",data);
	if(PDIN5== 1){
		data += 0x20;
//		d6 = PDIN5;
	}
//	printf("data6=0x%x\r\n",data);
	if(PDIN6== 1){
		data += 0x40;
//		d7 = PDIN6;
	}
	if(data == 0x7f){
//		printf("data7=0x%x\r\n",data);
		data = 0xff;
	}

	return data ;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ????: EXTI0_IRQHandler
** ????: ??0????			
** ????:?
** ?   ?: Dream
** ?   ?: 2011?6?20?
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!= RESET)	 //????????,????????1
	{
	//	gLED2 = getnum();
		gLED2_irq = 1;
		
	  LED1=1;
	  LED2=0;							 
	}
	EXTI_ClearITPendingBit(EXTI_Line0);		     //???????,??!!			
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: EXTI15_10_IRQHandler
** ��������: �ж�15_10��ں���			
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int gnum = 0;
void EXTI15_10_IRQHandler(void)
{
	int data = 0;
	if(EXTI_GetITStatus(EXTI_Line13)!= RESET)  	//�ж��Ƿ����жϣ��������жϱ�־��1
	{
	  LED1=0;
	  LED2=1;
	}
	if(EXTI_GetITStatus(EXTI_Line12)!= RESET)  	//�ж��Ƿ����жϣ��������жϱ�־��1
	{
#if 1
//		gLED2 = getnum();

/*		if(gnum %2 == 0){
			LED1=1;
			LED2=0;
		}
		else {
			LED1=0;
			LED2=1;
		}
		gnum ++; */
#endif		
	}
	if(EXTI_GetITStatus(EXTI_Line11)!= RESET)  	//�ж��Ƿ����жϣ��������жϱ�־��1
	{
	//	gLED1 = getnum2();
		gLED1_irq = 1;
/*		if(gnum %2 == 0){
			LED1=1;
			LED2=1;
		}
		else {
			LED1=0;
			LED2=0;
		}
		gnum ++;
		*/
	}
	
	EXTI_ClearITPendingBit(EXTI_Line13);		//����жϹ���λ����Ҫ����		
	EXTI_ClearITPendingBit(EXTI_Line12);		//����жϹ���λ����Ҫ����	
	EXTI_ClearITPendingBit(EXTI_Line11);		//����жϹ���λ����Ҫ����		
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Delay_Ms_Ms
** ��������: ��ʱ1MS (��ͨ���������ж�����׼ȷ��)			
** ����������time (ms) ע��time<65535
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Ms(uint16_t time)  //��ʱ����
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<10260;j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Delay_Ms_Us
** ��������: ��ʱ1us (��ͨ���������ж�����׼ȷ��)
** ����������time (us) ע��time<65535				 
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Us(uint16_t time)  //��ʱ����
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<9;j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
