//头文件调用
#include "usually.h"
#include "usart.h"
#include "stm32f10x_exti.h"
#include "time.h"
//变量定义
const char menu[] =
   "\n\r"
   "+********************* STM32开发板  ********************+\n\r"	   //"\n"：在超级终端的作用是换行
   "|                                                       |\n\r"	   //"\r" :在超级终端的作用是置顶
   "|                  USART 异步通信试验                   |\n\r"
   "|                   www.scdbit.com                      |\n\r"
   "|                   成都贝一特科技                      |\n\r"
   "|                      2012.5.15                        |\n\r"
   "|                技术支持QQ：406795003                  |\n\r"
   "+-------------------------------------------------------+\n\r";

//函数申明
void Init_LED(void);	
void Init_NVIC(void);
void Init_TI_KEY(void);
void Delay_Ms(uint16_t time);  
void Delay_Us(uint16_t time); 
int getshumaguannNum(int val);

#define Buf2_Max 200 					  //串口2缓存长度
/*************	本地常量声明	**************/
const char *string = "AT+CIPSTART=\"TCP\",\"116.31.51.113\",9015";	//IP登录服务器

/*************  本地变量声明	**************/
//char Uart1_Buf[Buf1_Max];
char Uart2_Buf[Buf2_Max];

u8 Times=0,First_Int = 0,shijian=0;
u16 Heartbeat=0;

vu8 Timer0_start;	//定时器0延时启动计数器
vu8 Uart2_Start;	//串口2开始接收数据
vu8 Uart2_End;	  //串口2接收数据结束
vu8 Heart_beat;		//发送心跳帧标志位



/*************	本地函数声明	**************/
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
** 函数名称: main
** 功能描述: 主函数入口
** 参数描述：这个程序演示了串口收发程序 ，接收是采用中断方式
				你可以输入键盘上的任意值 电脑超级终端将以十进制输出你输入的数值 
				比如：1对应十进制是49；2对应于50；a对应于97；这个你懂！！！
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int main(void)
{
	
	int num1 = 0, num2= 0, num3 = 0, num4 = 0;
	SystemInit();					//系统时钟配置
	Init_NVIC();					//中断向量表注册函数
	Init_LED();						//各个外设引脚配置
	Init_TI_KEY();				//?????????
//	Init_Usart();					//串口引脚配置
//	Usart_Configuration(115200);	//串口配置 设置波特率为115200
	USART1_Init_Config(115200);
#if HAVE_SIM800
	USART2_Init_Config(115200);
	Timer2_Init_Config();
	
	//Test_connet();
	
	printf("GPRS模块GPRS测试程序\r\n");
	printf("GPRS模块在注册网络\r\n");
	Wait_CREG();
	printf("GPRS模块注册成功\r\n");
	printf("GPRS模块开始连接服务器\r\n");
	Set_ATE0();
	Connect_Server();
	printf("连接成功\r\n");
#endif	

//	printf(menu);					//输出字符串
//	printf("贝一特助你开启STM32大门,让你轻松入门！！\n\r");
//	printf("请输入键盘上的任意字符，串口将以十进制输出你输入的字符\n\r");		   


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
// 获取数码管值
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
** 函数名称: Init_LED
** 功能描述: LED IO引脚配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE,ENABLE);	//使能各个端口时钟，重要！！！
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;										//配置LED端口挂接到6、12、13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				   	//根据参数初始化GPIOD寄存器
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			 //板上LED编号 D5
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
** 函数名称: Init_NVIC
** 功能描述: 系统中断配置
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年5月14日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_NVIC(void)
{ 	
  	NVIC_InitTypeDef NVIC_InitStructure;			//定义一个NVIC向量表结构体变量

	#ifdef  VECT_TAB_RAM  							//向量表基地址选择

	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//将0x20000000地址作为向量表基地址(RAM)
	#else  

	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //将0x08000000地址作为向量表基地址(FLASH)  
	#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置中断组 为2 
/*
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//配置串口1为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; 	//设置占先优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//设置副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//使能串口1中断
	NVIC_Init(&NVIC_InitStructure);							  	//根据参数初始化中断寄存器
*/	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//设定中断源为PC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//中断占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			////设定中断源为E0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//中断占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
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
** 函数名称: EXTI15_10_IRQHandler
** 功能描述: 中断15_10入口函数			
** 参数描述：无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int gnum = 0;
void EXTI15_10_IRQHandler(void)
{
	int data = 0;
	if(EXTI_GetITStatus(EXTI_Line13)!= RESET)  	//判断是否发生中断，发生则中断标志置1
	{
	  LED1=0;
	  LED2=1;
	}
	if(EXTI_GetITStatus(EXTI_Line12)!= RESET)  	//判断是否发生中断，发生则中断标志置1
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
	if(EXTI_GetITStatus(EXTI_Line11)!= RESET)  	//判断是否发生中断，发生则中断标志置1
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
	
	EXTI_ClearITPendingBit(EXTI_Line13);		//清楚中断挂起位，重要！！		
	EXTI_ClearITPendingBit(EXTI_Line12);		//清楚中断挂起位，重要！！	
	EXTI_ClearITPendingBit(EXTI_Line11);		//清楚中断挂起位，重要！！		
}



/*******************************************************************************
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	u16 k;
	for(k=0;k<Buf2_Max;k++)      //将缓存内容清零
	{
		Uart2_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}

/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

u8 Find(char *a)
{ 
  if(strstr(Uart2_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}

/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,u8 wait_time)         
{
	u8 i;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//将字符串地址给b
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
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
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
		printf("注册中.....\r\n");
	}
}


/*******************************************************************************
* 函数名 : Test_connet
* 描述   : 测试连接
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
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
* 函数名 : Set_ATE0
* 描述   : 取消回显
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Set_ATE0(void)
{
	Second_AT_Command("ATE0","OK",3);								//取消回显		
}
/*******************************************************************************
* 函数名 : Connect_Server
* 描述   : GPRS连接服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Connect_Server(void)
{
	UART2_SendString("AT+CIPCLOSE=1");	//关闭连接
  Delay_Ms(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//设置GPRS移动台类别为B,支持包交换和数据交换 
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//设置PDP上下文,互联网接协议,接入点等信息
	Second_AT_Command("AT+CGATT=1","OK",2);//附着GPRS业务
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//设置为GPRS连接模式
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
	Second_AT_Command((char*)string,"OK",5);
	Delay_Ms(100);
	CLR_Buf2();
}
/*******************************************************************************
* 函数名 : Rec_Server_Data
* 描述   : 接收服务器数据函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Rec_Server_Data(void)
{
	if(strstr(Uart2_Buf,"+IPD")!=NULL)   		//若缓存字符串中含有^SISR
	{	
		Heartbeat=0;	//清除心跳帧计数器
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
		printf("收到新信息：\r\n");
		printf(Uart2_Buf);
		CLR_Buf2();
		Heart_beat=1;//发送应答数据，告诉服务器收到数据		
	}
}
/*******************************************************************************
* 函数名 : Send_OK
* 描述   : 发送数据应答服务器的指令，该函数在有两功能
					1：接收到服务器的数据后，应答服务器
					2：服务器无下发数据时，每隔10秒发送一帧心跳，保持与服务器连接
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Send_OK(void)
{
	Second_AT_Command("AT+CIPSEND",">",2);
	Second_AT_Command("OK\32\0","SEND OK",8);;			//回复OK 
}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Ms
** 功能描述: 延时1MS (可通过仿真来判断他的准确度)			
** 参数描述：time (ms) 注意time<65535
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Ms(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<10260;j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Delay_Ms_Us
** 功能描述: 延时1us (可通过仿真来判断他的准确度)
** 参数描述：time (us) 注意time<65535				 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Us(uint16_t time)  //延时函数
{ 
	uint16_t i,j;
	for(i=0;i<time;i++)
  		for(j=0;j<9;j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
