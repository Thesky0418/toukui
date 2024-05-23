#ifndef   __DHT11_H
#define   __DHT11_H
 
#include "main.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"

extern char str[];

#define DHT11_DATA_OUT_Pin GPIO_PIN_6
#define DHT11_DATA_OUT_GPIO_Port GPIOB
#define CPU_FREQUENCY_MHZ    72		// STM32Ê±ÖÓÖ÷Æµ
#define DelayXms delay_ms
#define DelayUs  delay_us

void DHT11_Rst(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi,uint8_t *tem,uint8_t *hum);
uint8_t DHT11_Init(void);
void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);
void delay_us(__IO uint32_t delay);
void delay_ms(__IO uint32_t delay);
#endif // DEBUG

