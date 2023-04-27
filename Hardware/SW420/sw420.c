#include "sw420.h"
#include "delay.h"

void sw420_Init(void)
{
 //PB0��ȡ����״̬
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 //
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 //IO���ٶ�Ϊ10MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA3
 GPIO_SetBits(GPIOA,GPIO_Pin_3);	
}

int check_shake(void)
{
//	delay_ms(1000);
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1)
	{
		return 1;
	}
	else 
		return 0;
}


