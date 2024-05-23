#ifndef _MY_U8G2_CONFIG_H_
#define _MY_U8G2_CONFIG_H_

#include "u8g2.h"
#include "stm32f1xx_hal.h"
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "dht11.h"
#define CPU_FREQUENCY_MHZ    72		// STM32Ê±ÖÓÖ÷Æµ

void MY_U8G2_IIC_Init(void);
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
void u8g2Init(u8g2_t *u8g2);
void u8g2_test_show(u8g2_t *u8g2);
void u8g2_test_1(u8g2_t *u8g2);



#endif
