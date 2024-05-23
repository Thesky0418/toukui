#include "dht11.h"
#define DHT11_DQ_IN HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_SET)	  //����

#define TEST 0

//��ʼ��DHT11��ͬʱ����Ƿ�������DHT11��PA11��ʼ��
 	 
uint8_t DHT11_Init(void)
{	 
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = DHT11_DATA_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_DATA_OUT_GPIO_Port, &GPIO_InitStruct);	

	DHT11_Rst();  
	return DHT11_Check();
}      
//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_RESET); 	//����
	DelayXms(20);    	//������ʱ����18ms
	HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_SET); 	//DQ=1������ 
	DelayUs(30);     	//������ʱ����20~40us
}
 
//����Ӧ
//����1��������
//����0�����ɹ�
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();//SET INPUT	 
  while (HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//DHT11����40~80us
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//DHT11�ٴ�����40~80us
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
 
//��ȡһ��λBit
//����1��0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//�ȴ���͵�ƽ
	{
		retry++;
		DelayUs(1);
	}
	retry=0;
	while(!HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		DelayUs(1);
	}
	DelayUs(40);//�ȴ�40us
	if(HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin))return 1;
	else return 0;		   
}
 
//��ȡһ���ֽ�
//���ض���������
uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}
 
//DHT11��ȡһ������
//temp:�¶�(��Χ:0~50��)
//humi:ʪ��(��Χ:20%~90%)
//tem���¶�С��λ
//hum��ʪ��С��λ
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi,uint8_t *tem,uint8_t *hum)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ�ֽ�
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*hum=buf[1];
			*temp=buf[2];
			*tem=buf[3];
		}
		#if TEST
		sprintf(str,"temp:%d,humi:%d\r\n",buf[2],buf[0]);
		HAL_UART_Transmit(&huart1,(uint8_t *)str,strlen(str),HAL_MAX_DELAY);    //ʪ��
		#endif
	}
	else return 1;
	return 0;	    
}
 

//DHT11���ģʽ����
void DHT11_IO_OUT(void)	
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = DHT11_DATA_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_DATA_OUT_GPIO_Port, &GPIO_InitStruct);
}
 
//DHT11����ģʽ����
void DHT11_IO_IN(void)	
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = DHT11_DATA_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_DATA_OUT_GPIO_Port, &GPIO_InitStruct);
}


void delay_us(__IO uint32_t delay)
{
    int last, curr, val;
    int temp;

    while (delay != 0)
    {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHZ * temp;
        if (curr >= 0)
        {
            do
            {
                val = SysTick->VAL;
            }
            while ((val < last) && (val >= curr));
        }
        else
        {
            curr += CPU_FREQUENCY_MHZ * 1000;
            do
            {
                val = SysTick->VAL;
            }
            while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}

void delay_ms(__IO uint32_t delay)
{
	for(int i=0;i<delay;i++)
	{
		delay_us(1000);
	}
}

