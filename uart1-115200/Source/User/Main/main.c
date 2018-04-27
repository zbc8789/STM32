//ͷ�ļ�����
#include "usually.h"
#include "usart.h"
#include "stm32f10x_exti.h"
#include "time.h"
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

#define Buf2_Max 200 					  //����2���泤��
/*************	���س�������	**************/
const char *string = "AT+CIPSTART=\"TCP\",\"116.31.51.113\",9015";	//IP��¼������

/*************  ���ر�������	**************/
//char Uart1_Buf[Buf1_Max];
char Uart2_Buf[Buf2_Max];

u8 Times=0,First_Int = 0,shijian=0;
u16 Heartbeat=0;

vu8 Timer0_start;	//��ʱ��0��ʱ����������
vu8 Uart2_Start;	//����2��ʼ��������
vu8 Uart2_End;	  //����2�������ݽ���
vu8 Heart_beat;		//��������֡��־λ



/*************	���غ�������	**************/
void CLR_Buf2(void);
u8 Find(char *a);
void Second_AT_Command(char *b,char *a,u8 wait_time);
void Set_ATE0(void);
void Connect_Server(void);
void Rec_Server_Data(void);
void Wait_CREG(void);
void Send_OK(void);

void Test_connet(void);

int getnum(void);
int gLED1 = 0xff;
int gLED2 = 0xff;

int gReal_Num1 = 0xff;
int gReal_Num2 = 0xff;

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
	
	int num1 = 0, num2= 0, num3 = 0, num4 = 0;
	SystemInit();					//ϵͳʱ������
	Init_NVIC();					//�ж�������ע�ắ��
	Init_LED();						//����������������
	Init_TI_KEY();				//?????????
//	Init_Usart();					//������������
//	Usart_Configuration(115200);	//�������� ���ò�����Ϊ115200
	USART1_Init_Config(115200);
#if HAVE_SIM800
	USART2_Init_Config(115200);
	Timer2_Init_Config();
	
	//Test_connet();
	
	printf("GPRSģ��GPRS���Գ���\r\n");
	printf("GPRSģ����ע������\r\n");
	Wait_CREG();
	printf("GPRSģ��ע��ɹ�\r\n");
	printf("GPRSģ�鿪ʼ���ӷ�����\r\n");
	Set_ATE0();
	Connect_Server();
	printf("���ӳɹ�\r\n");
#endif	

//	printf(menu);					//����ַ���
//	printf("��һ�����㿪��STM32����,�����������ţ���\n\r");
//	printf("����������ϵ������ַ������ڽ���ʮ���������������ַ�\n\r");		   


	while(1)													
	{ 

#if 0	
		if(BAIJING_FLAG == 0){
			continue;
		}
#endif		
//		printf("BAIJING_FLAG == %d\r\n",BAIJING_FLAG);
		
#if HAVE_SIM800
		Rec_Server_Data();
				
		if(Heart_beat)
		{
			Send_OK();
			Heart_beat=0;
		}
#endif		


		if(gLED1_irq == 1)
		{
			gLED1_irq = 0;
			gLED1 = getnum();
//			printf("gLED1=%d\r\n",gLED1);
		}
		
		if(gLED2_irq == 1){
			gLED2_irq = 0;
			gLED2 = getnum();
//			printf("gLED2=%d\r\n",gLED2);
		}
		
		if(gLED1 != 0xff ){
			if(num1 > 20){
				//printf("\r\n[1]LED1 = %d \r\n",getshumaguannNum(gLED1));
				gReal_Num1 = getshumaguannNum(gLED1);
//				printf("\r\n[1]gReal_Num1 = %d \r\n", gReal_Num1);
				num1 = 0;
			}
			num1 ++;
			gLED1 = 0xff;
			//gLED2 = 0xff;
		}
		if(gLED2 != 0xff){
			
			if(num2 > 20){
			//	printf("\r\n[2]led2 = %d \r\n", getshumaguannNum(gLED2));
				gReal_Num2 = getshumaguannNum(gLED2);
//				printf("\r\n[2]gReal_Num2 = %d \r\n", gReal_Num2);
				num2 = 0;
			}
			num2 ++;
			//gLED1 = 0xff;
			gLED2 = 0xff;
		}
		
		if(num3 > (10260*500) && (BAIJING_FLAG == 1)){
			
			printf("num = %d\r\n",gReal_Num1 + gReal_Num2 *10);
			
			num3 = 0;

		}
		
		num3 ++;	
	}
}
// ��ȡ�����ֵ
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
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			 //C13
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
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
/*
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//���ô���1Ϊ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 	//����ռ�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//���ø����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//ʹ�ܴ���1�ж�
	NVIC_Init(&NVIC_InitStructure);							  	//���ݲ�����ʼ���жϼĴ���
*/	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//�趨�ж�ԴΪPC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�ж�ռ���ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);							   	//���ݲ�����ʼ���жϼĴ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			////�趨�ж�ԴΪE0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//�ж�ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);	
	
	
}
int getnum(void){
//	char d1 = 0, d2 = 0, d3 =0,  d5 = 0, d6 = 0, d7 = 0;
	int data = 0;
//	char d4 = 0;
	
	Delay_Us(10);
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
	if(data == 0x7f){
		data = 0xff;
	}
	else{
#if 0
		printf("data7=0x%x\r\n",data);
#endif
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
//		printf("1");
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
	 	LED1=0;
	  	LED2=1;
//		printf("2");
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



/*******************************************************************************
* ������ : CLR_Buf2
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //��������������
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u8 wait_time)         
{
	u8 i;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
				for (; *b!='\0';b++)
				{
					while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
					USART_SendData(USART2,*b);//UART2_SendData(*b);
				}
				UART2_SendLR();	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Wait_CREG(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf2();
  	while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT+CREG?");
		UART2_SendLR();
		Delay_Ms(5000);  						
	  for(k=0;k < Buf2_Max;k++)      			
    {
    	printf("Uart2_Buf[k] == %s \r\n", Uart2_Buf);
			if(Uart2_Buf[k] == ':')
			{
//				printf("Uart2_Buf[k+4] == %c \r\n", Uart2_Buf[k+4]);
				//if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '5'))
				if((Uart2_Buf[k+4] == '1')||(Uart2_Buf[k+4] == '3'))
				{
					i = 1;
					//UART1_SendLR();
					printf("\r\n");
				  break;
				}
			}
			CLR_Buf2();
		}
		printf("ע����.....\r\n");
	}
}


/*******************************************************************************
* ������ : Test_connet
* ����   : ��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Test_connet(void)
{
	u8 i;
	u8 k;
	i = 0;
	CLR_Buf2();
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART2_SendString("AT");
		UART2_SendLR();
		Delay_Ms(50);  						
	    for(k=0;k < Buf2_Max;k++)      			
    	{
   // 		printf("\r\nUart2_Buf[%d] = 0x%x\r\n",k, Uart2_Buf[k]);
		}
	}
}

/*******************************************************************************
* ������ : Set_ATE0
* ����   : ȡ������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3);								//ȡ������		
}
/*******************************************************************************
* ������ : Connect_Server
* ����   : GPRS���ӷ���������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Connect_Server(void)
{
	UART2_SendString("AT+CIPCLOSE=1");	//�ر�����
  Delay_Ms(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//�ر��ƶ�����
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//����PDP������,��������Э��,��������Ϣ
	Second_AT_Command("AT+CGATT=1","OK",2);//����GPRSҵ��
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//����ΪGPRS����ģʽ
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
	Second_AT_Command((char*)string,"OK",5);
	Delay_Ms(100);
	CLR_Buf2();
}
/*******************************************************************************
* ������ : Rec_Server_Data
* ����   : ���շ��������ݺ���
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(strstr(Uart2_Buf,"+IPD")!=NULL)   		//�������ַ����к���^SISR
	{	
		Heartbeat=0;	//�������֡������
		Heart_beat=0;
		Delay_Ms(100);
		if(strstr(Uart2_Buf,"onled")!=NULL)
		{
			LED1 = 1;
		}
		else if(strstr(Uart2_Buf,"offled")!=NULL)
		{
			LED1 = 0;
		}
		printf("�յ�����Ϣ��\r\n");
		printf(Uart2_Buf);
		CLR_Buf2();
		Heart_beat=1;//����Ӧ�����ݣ����߷������յ�����		
	}
}
/*******************************************************************************
* ������ : Send_OK
* ����   : ��������Ӧ���������ָ��ú�������������
					1�����յ������������ݺ�Ӧ�������
					2�����������·�����ʱ��ÿ��10�뷢��һ֡���������������������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Send_OK(void)
{
	Second_AT_Command("AT+CIPSEND",">",2);
	Second_AT_Command("OK\32\0","SEND OK",8);;			//�ظ�OK 
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
