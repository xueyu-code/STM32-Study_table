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
//һЩ����RC522�õ������ݶ���
unsigned char CT[2];//������
unsigned char SN[4]; //�����Ŀ���
unsigned char UID[4]={163,93,49,161};//д���Ŀ���
unsigned char RFID[16];			//���RFID 
u8 data_out[16]={0,0,0,1,0,0,1,0,0,0,0,0};
int card_flag=0;//�����Ƭͨ��У����Ϊ1
u8 status;
int rfid_flag=0;//�Ƿ�����ĵ�����־λ Ĭ��Ϊ0
int card_check=0;//�ȽϿ����Ƿ���ͬ�ĵ�����־λ Ĭ��Ϊ0�������У��ʧ�ܾͱ�Ϊ-1������4λ�ۼӼ��ɽ�������У��
int read_card_flag=0;//��⵽��֮���Ϊ1

int send=0;
int shake_flag=0;

float Pitch,Roll,Yaw;
 int main(void)
 {	

		int study_flag=0;
		delay_init();	    	 //��ʱ������ʼ��	  
		NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	  uart_init(9600); //����1��ʼ��������ģ�����ͨ��
//		max_volume();
	  InitRc522();//��ʼ��RC522
		sw420_Init();
	 
		IIC_Init();
		MPU6050_initialize();     //=====MPU6050��ʼ��	
		DMP_Init();
		TIM2_Getsample_Int(5000,719);		//50ms����ʱ�ж� Ĭ����1999 719 
	 //��ʱ���ж������Ƕ�ȡ���
	 
		OLED_Init();			//��ʼ��OLED  
		OLED_Clear();
		OLED_DrawBMP(0,0,128,8,BMP2);//��ʾ ������ѧϰ 
		OLED_ShowString(0,6,"Angle:");
	while(1) 
	{		
			card_check=rfid_check();//���IC���Ƿ���������Ҫ�Ŀ�
//			shake_flag=check_shake();//�𶯼��
//			Read_DMP(&Pitch,&Roll,&Yaw);
			//mpu6050��ȡ����
			OLED_ShowNum(50,6,Pitch,3,16);
			if(card_check==1)
			{
				pleasr_study();
				
				study_flag=1;
				card_check=0;
				
			}
			
			//��ȡ��״̬
//			if((study_flag==1)&&(shake_flag==1)) //��ʱ����ߵ�ƽ
//			{

//				please_steady();
//			
//			}
			
				if(study_flag==1) //��ʱ����ߵ�ƽ
			{

				shake_flag=check_shake();
				if(shake_flag==1)
					please_steady();
			
			}
			
			
	}	  	
}
 

//����ȡ���Ŀ��Ƿ��������ǵ�����
int rfid_check(void)
{
	int j=0;
		status = PcdRequest(PICC_REQALL,CT);
			if(status==MI_OK)//�����ɹ�
			{
			 status=MI_ERR;
			 status = PcdAnticoll(SN);		
			}
			if (status==MI_OK)//���nײ�ɹ�
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
			return -1;//����-1��ʾУ��ʧ��
		}
			if(read_card_flag==1&&rfid_flag==0)
				{
			return 1;
				}
}
