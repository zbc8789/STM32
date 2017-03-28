//头文件调用
#include "usually.h"
#include "usart.h"
#include "stm32f10x_exti.h"
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
	SystemInit();					//系统时钟配置
	Init_NVIC();					//中断向量表注册函数
	Init_LED();						//各个外设引脚配置
	Init_TI_KEY();				//?????????
	Init_Usart();					//串口引脚配置
	Usart_Configuration(115200);	//串口配置 设置波特率为115200
	printf(menu);					//输出字符串
	printf("贝一特助你开启STM32大门,让你轻松入门！！\n\r");
	printf("请输入键盘上的任意字符，串口将以十进制输出你输入的字符\n\r");		   
	while(1)													
	{ 
			;
//		LED1=~LED1;	   				
//		Delay_Ms(200);				 //LED1闪烁，系统正在运行
	}
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
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			 //板上LED编号 D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			 //板上LED编号 D5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	

	
	// D0 --D5  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			 // D0
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			 //D1
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			 //D2
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			 //D3
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			 //D4
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			 //D5
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			 //D6
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
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

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//配置串口1为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//设置占先优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//设置副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//使能串口1中断
	NVIC_Init(&NVIC_InitStructure);							  	//根据参数初始化中断寄存器
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		//设定中断源为PC13
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//中断占优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//副优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能中断
	NVIC_Init(&NVIC_InitStructure);							   	//根据参数初始化中断寄存器
}
int getnum(void){
	char d1 = 0, d2 = 0, d3 =0,  d5 = 0, d6 = 0, d7 = 0;
	int data = 0;
	char d4 = 0;
	
	Delay_Ms(2);
	if(PDIN0 == 1){
		data += 0x01;
		d1 = PDIN0;
	}
//	printf("data1=0x%x\r\n",data);
	if(PDIN1 == 1){
		data += 0x02;
		d2 = PDIN1;
	}
//	printf("data2=0x%x\r\n",data);
	if(PDIN2 == 1){
		data |= 0x04;
		d3 = PDIN2;
	}
//	printf("data3=0x%x\r\n",data);
	if(PDIN3 == 1){
		data += 0x8;
		d4 = PDIN3;
	}
//	printf("data4=0x%x\r\n",data);
	if(PDIN4== 1){
		data += 0x10;
		d5 = PDIN4;
	}
//	printf("data5=0x%x\r\n",data);
	if(PDIN5== 1){
		data += 0x20;
		d6 = PDIN5;
	}
//	printf("data6=0x%x\r\n",data);
	if(PDIN6== 1){
		data += 0x40;
		d7 = PDIN6;
	}
	if(data != 0x7f)
		printf("data7=0x%x\r\n",data);

	return data ;
}

int getnum2(void){
	char d1 = 0, d2 = 0, d3 =0,  d5 = 0, d6 = 0, d7 = 0;
	int data = 0;
	char d4 = 0;
	
	Delay_Ms(10);
	if(PDIN0 == 1){
		data += 0x01;
		d1 = PDIN0;
	}
//	printf("data1=0x%x\r\n",data);
	if(PDIN1 == 1){
		data += 0x02;
		d2 = PDIN1;
	}
//	printf("data2=0x%x\r\n",data);
	if(PDIN2 == 1){
		data |= 0x04;
		d3 = PDIN2;
	}
//	printf("data3=0x%x\r\n",data);
	if(PDIN3 == 1){
		data += 0x8;
		d4 = PDIN3;
	}
//	printf("data4=0x%x\r\n",data);
	if(PDIN4== 1){
		data += 0x10;
		d5 = PDIN4;
	}
//	printf("data5=0x%x\r\n",data);
	if(PDIN5== 1){
		data += 0x20;
		d6 = PDIN5;
	}
//	printf("data6=0x%x\r\n",data);
	if(PDIN6== 1){
		data += 0x40;
		d7 = PDIN6;
	}
	if(data != 0x7f)
		printf("data7=0x%x\r\n",data);

	return data ;
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
#if 0
			data = getnum();
		if(gnum %2 == 0){
			LED1=1;
			LED2=0;
		}
		else {
			LED1=0;
			LED2=1;
		}
		gnum ++;
#endif		
	}
	if(EXTI_GetITStatus(EXTI_Line11)!= RESET)  	//判断是否发生中断，发生则中断标志置1
	{
		data = getnum2();
		if(gnum %2 == 0){
			LED1=1;
			LED2=1;
		}
		else {
			LED1=0;
			LED2=0;
		}
		gnum ++;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line13);		//清楚中断挂起位，重要！！		
	EXTI_ClearITPendingBit(EXTI_Line12);		//清楚中断挂起位，重要！！	
	EXTI_ClearITPendingBit(EXTI_Line11);		//清楚中断挂起位，重要！！		
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
