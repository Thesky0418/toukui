#include "dht11.h"
#define DHT11_DQ_IN HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_SET)	  //输入

#define TEST 0

//初始化DHT11，同时检测是否连接上DHT11，PA11初始化
 	 
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
//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_RESET); 	//拉低
	DelayXms(20);    	//拉低延时至少18ms
	HAL_GPIO_WritePin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin, GPIO_PIN_SET); 	//DQ=1，拉高 
	DelayUs(30);     	//拉高延时至少20~40us
}
 
//检测回应
//返回1：检测错误
//返回0：检测成功
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();//SET INPUT	 
  while (HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//DHT11拉低40~80us
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//DHT11再次拉高40~80us
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
 
//读取一个位Bit
//返回1或0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//等待变低电平
	{
		retry++;
		DelayUs(1);
	}
	retry=0;
	while(!HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin)&&retry<100)//等待变高电平
	{
		retry++;
		DelayUs(1);
	}
	DelayUs(40);//等待40us
	if(HAL_GPIO_ReadPin(DHT11_DATA_OUT_GPIO_Port, DHT11_DATA_OUT_Pin))return 1;
	else return 0;		   
}
 
//读取一个字节
//返回读到的数据
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
 
//DHT11读取一次数据
//temp:温度(范围:0~50°)
//humi:湿度(范围:20%~90%)
//tem：温度小数位
//hum：湿度小数位
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi,uint8_t *tem,uint8_t *hum)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位字节
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
		HAL_UART_Transmit(&huart1,(uint8_t *)str,strlen(str),HAL_MAX_DELAY);    //湿度
		#endif
	}
	else return 1;
	return 0;	    
}
 

//DHT11输出模式配置
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
 
//DHT11输入模式配置
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

