
#include "my_u8g2_config.h"

#define IIC_GPIO_Port GPIOA

#define u16 unsigned int
uint8_t temp1=20,hum1=60;
uint8_t str1[70],str2[20],str3[20],str4[20];
extern float smell;
extern float Light; //光照庿
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __nop();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __nop();
        }
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        HAL_Delay(1);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        delay_us(5);
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
		if(arg_int == 1) 
		{
			HAL_GPIO_WritePin(IIC_GPIO_Port, SCL_Pin,GPIO_PIN_SET);
		}
		else if(arg_int == 0)
		{
			HAL_GPIO_WritePin(IIC_GPIO_Port, SCL_Pin,GPIO_PIN_RESET);  
		}  
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        if(arg_int == 1) 
		{
			HAL_GPIO_WritePin(IIC_GPIO_Port, SDA_Pin,GPIO_PIN_SET);
		}
		else if(arg_int == 0)
		{
			HAL_GPIO_WritePin(IIC_GPIO_Port, SDA_Pin,GPIO_PIN_RESET);  
		} 
        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

void u8g2Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay); 
	u8g2_InitDisplay(u8g2); 
	u8g2_SetPowerSave(u8g2, 0); 
	u8g2_ClearBuffer(u8g2);
}

void u8g2_test_show(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1); 
    u8g2_SetFontDirection(u8g2, 0); 
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); 
    u8g2_DrawStr(u8g2, 0, 20, "U");
    
    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21,8,"8");
        
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51,30,"g");
    u8g2_DrawStr(u8g2, 67,30,"\xb2");
    
    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);
  
    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");
}
void u8g2_test_1(u8g2_t *u8g2)
{
		
    u8g2_SetFont(u8g2, u8g2_font_6x10_tr); 
		sprintf((char*)str1,"temp:%d,humi:%d",temp,humi);
//		sprintf((char*)str2,"food:%d,water:%d",sl,sw);
//		sprintf((char*)str3,"fan:%d,feed:%d",fs,dj);
//		sprintf((char*)str4,"pump:%d",sb);
		u8g2_DrawStr(u8g2, 0,15,(char*)str1);
//    u8g2_DrawStr(u8g2, 0,25,(char*)str2);
//		u8g2_DrawStr(u8g2, 0,35,(char*)str3);
//		u8g2_DrawStr(u8g2, 0,45,(char*)str4);
}

