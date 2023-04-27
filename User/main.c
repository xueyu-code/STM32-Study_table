#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "rc522.h"
#include "usart.h"
#include "sw420.h"
#include "dysv5w.h"
#include "mpu6050.h"
#include "TIME.h"

int rfid_check(void);
//一些关于RC522用到的数据定义
unsigned char CT[2];//卡类型
unsigned char SN[4]; //读出的卡号
unsigned char UID[4]={163,93,49,161};//写死的卡号
unsigned char RFID[16];			//存放RFID 
u8 data_out[16]={0,0,0,1,0,0,1,0,0,0,0,0};
int card_flag=0;//如果卡片通过校验则为1
u8 status;
int rfid_flag=0;//是否读卡的单个标志位 默认为0
int card_check=0;//比较卡号是否相同的单个标志位 默认为0，如果有校验失败就变为-1，这样4位累加即可进行最后的校验
int read_card_flag=0;//检测到卡之后变为1

int send=0;
int shake_flag=0;

float Pitch,Roll,Yaw;
 int main(void)
 {	

		int study_flag=0;
		delay_init();	    	 //延时函数初始化	  
		NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	  uart_init(9600); //串口1初始化与语音模组进行通信
//		max_volume();
	  InitRc522();//初始化RC522
		sw420_Init();
	 
		IIC_Init();
		MPU6050_initialize();     //=====MPU6050初始化	
		DMP_Init();
		TIM2_Getsample_Int(5000,719);		//50ms任务定时中断 默认是1999 719 
	 //定时器中断任务是读取倾角
	 
		OLED_Init();			//初始化OLED  
		OLED_Clear();
		OLED_DrawBMP(0,0,128,8,BMP2);//显示 请认真学习 
		OLED_ShowString(0,6,"Angle:");
	while(1) 
	{		
			card_check=rfid_check();//检测IC卡是否是我们需要的卡
//			shake_flag=check_shake();//震动检测
//			Read_DMP(&Pitch,&Roll,&Yaw);
			//mpu6050获取数据
			OLED_ShowNum(50,6,Pitch,3,16);
			if(card_check==1)
			{
				pleasr_study();
				
				study_flag=1;
				card_check=0;
				
			}
			
			//读取震动状态
//			if((study_flag==1)&&(shake_flag==1)) //震动时输出高电平
//			{

//				please_steady();
//			
//			}
			
				if(study_flag==1) //震动时输出高电平
			{

				shake_flag=check_shake();
				if(shake_flag==1)
					please_steady();
			
			}
			
			
	}	  	
}
 

//检测读取到的卡是否满足我们的需求
int rfid_check(void)
{
	int j=0;
		status = PcdRequest(PICC_REQALL,CT);
			if(status==MI_OK)//尋卡成功
			{
			 status=MI_ERR;
			 status = PcdAnticoll(SN);		
			}
			if (status==MI_OK)//防衝撞成功
			{
				read_card_flag=1;
				for(j=0;j<4;j++)
				{
					if(UID[j]==SN[j])
						rfid_flag+=rfid_flag;
					else
					{rfid_flag=-1;
						rfid_flag+=rfid_flag;}
				}
			}
			
		if(read_card_flag==1&&rfid_flag<0)
		{
			return -1;//返回-1表示校验失败
		}
			if(read_card_flag==1&&rfid_flag==0)
				{
			return 1;
				}
}
