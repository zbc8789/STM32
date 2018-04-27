//头文件调用
#include "usually.h"
#include "usart.h"

extern u8 First_Int;
extern char Uart2_Buf[Buf2_Max];

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int Data, FILE *f)
{   
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART1,Data);						  //发送一个字符
	   
	return Data;										  //返回一个值
}
#endif 

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_Send_Byte
** 功能描述: 串口发送一个字符串
** 参数描述：Data 要发送的数据
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_Send_Byte(uint16_t Data)
{ 
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART1,Data);						  //发送一个字符
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_Send_Byte
** 功能描述: 串口发送一个字符串
** 参数描述：Data 要发送的数据
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
uint8_t USART1_Receive_Byte(void)
{ 
   	while(!(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))); //USART_GetFlagStatus：得到接收状态位
														   //USART_FLAG_RXNE:接收数据寄存器非空标志位 
														   //1：忙状态  0：空闲(没收到数据，等待。。。)
	return USART_ReceiveData(USART1);					   //接收一个字符
}



/*******************************************************************************
* 函数名  : USART1_Init_Config
* 描述    : USART1初始化配置
* 输入    : bound：波特率(常用：2400、4800、9600、19200、38400、115200等)
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void USART1_Init_Config(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*使能USART1和GPIOA外设时钟*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	

	/*复位串口1*/
 	USART_DeInit(USART1);  
	
	/*USART1_GPIO初始化设置*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//USART1_TXD(PA.9)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				//USART1_RXD(PA.10)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)


   /*USART1 初始化设置*/
	USART_InitStructure.USART_BaudRate = bound;										//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//工作模式设置为收发模式
    USART_Init(USART1, &USART_InitStructure);										//初始化串口1

   /*Usart1 NVIC配置*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器 
	  
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//使能串口1接收中断

    USART_Cmd(USART1, ENABLE);                    			//使能串口 
	USART_ClearFlag(USART1, USART_FLAG_TC);					//清除发送完成标志
}


/*******************************************************************************
* 函数名  : USART2_Init_Config
* 描述    : USART3初始化配置
* 输入    : bound：波特率(常用：2400、4800、9600、19200、38400、115200等)
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void USART2_Init_Config(u32 bound)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	/*使能USART2外设时钟*/  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/*复位串口2*/
 	USART_DeInit(USART2);  
	
	/*USART2_GPIO初始化设置*/ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			//USART2_TXD(PA.2)     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9)  
   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2_RXD(PA.3)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);					//调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)


   /*USART2 初始化设置*/
	USART_InitStructure.USART_BaudRate = bound;										//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//工作模式设置为收发模式
  USART_Init(USART2, &USART_InitStructure);										//初始化串口2

   /*Usart1 NVIC配置*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//从优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器 
	  
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//使能串口2接收中断

  USART_Cmd(USART2, ENABLE);                    			//使能串口 
	USART_ClearFlag(USART2, USART_FLAG_TC);					//清除发送完成标志
}



/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Usart_Init
** 功能描述: 串口引脚初始化
** 参数描述: 无
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_Usart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);	
															//使能各个端口时钟，重要！！！

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 				//配置串口接收端口挂接到9端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		//复用功能输出开漏
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	//根据参数初始化GPIOA寄存器	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入(复位状态);	   				
  	GPIO_Init(GPIOA, &GPIO_InitStructure);				   	//根据参数初始化GPIOA寄存器	
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Usart_Configuration
** 功能描述: 串口配置函数
** 参数描述: BaudRate设置波特率 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Usart_Configuration(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;							    	//定义一个串口结构体

	USART_InitStructure.USART_BaudRate            =BaudRate ;	  			//波特率115200
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b; 	//传输过程中使用8位数据
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;	 	//在帧结尾传输1位停止位
	USART_InitStructure.USART_Parity              = USART_Parity_No ;	 	//奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流失能
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式
	USART_Init(USART1, &USART_InitStructure);								//根据参数初始化串口寄存器
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);							//使能串口中断接收
	USART_Cmd(USART1, ENABLE);     											//使能串口外设
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_IRQHandler
** 功能描述: 串口中断函数
** 参数描述: 无 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_IRQHandler()
{
	uint8_t ReceiveDate;								//定义一个变量存放接收的数据
	if(!(USART_GetITStatus(USART1,USART_IT_RXNE))); 	//读取接收中断标志位USART_IT_RXNE 
														//USART_FLAG_RXNE:接收数据寄存器非空标志位 
														//1：忙状态  0：空闲(没收到数据，等待。。。)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	//清楚中断标志位
		ReceiveDate=USART_ReceiveData(USART1);			//接收字符存入数组
		printf("%d",(char*)ReceiveDate);				//以十进制输出输入的值,从这里我们可以看到键盘上所有的键值对应的十进制数
														//比如输入"1"对应的输出49 、 输入"A" 对应的值为65
		printf("\n\r");									//换行置顶
	}  
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART2_IRQHandler
** 功能描述: 串口中断函数
** 参数描述: 无 
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART2_IRQHandler()
{
	uint8_t ReceiveDate;								//定义一个变量存放接收的数据
	if(!(USART_GetITStatus(USART2,USART_IT_RXNE))); 	//读取接收中断标志位USART_IT_RXNE 
														//USART_FLAG_RXNE:接收数据寄存器非空标志位 
														//1：忙状态  0：空闲(没收到数据，等待。。。)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//清楚中断标志位
		ReceiveDate=USART_ReceiveData(USART2);			//接收字符存入数组
		if((ReceiveDate == 0x0d) || (ReceiveDate == 0x0a)){
			return;
		}
		Uart2_Buf[First_Int] = ReceiveDate;  	  //将接收到的字符串存到缓存中
		First_Int++;                			//缓存指针向后移动
		if(First_Int > Buf2_Max)       		//如果缓存满,将缓存指针指向缓存的首地址
		{
			First_Int = 0;
		}
		
//		printf("%c ",(char*)ReceiveDate);				//以十进制输出输入的值,从这里我们可以看到键盘上所有的键值对应的十进制数
														//比如输入"1"对应的输出49 、 输入"A" 对应的值为65
//		printf("\n\r");									//换行置顶
	} 														
}

/*******************************************************************************
* 函数名  : UART2_SendString
* 描述    : USART2发送字符串
* 输入    : *s字符串指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void UART2_SendString(char* s)
{
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//发送当前字符
	}
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
