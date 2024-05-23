#include "sim.h"

char S_data[300];
extern uint32_t temp;
extern uint32_t humi;
extern uint32_t mq135;
extern uint32_t mq2;
extern uint8_t people;

void SIM_Send_str(char *str)
{
    HAL_UART_Transmit(&huart2,(uint8_t *)str,sizeof(str),1000);
}

void SIM_Send_val(uint8_t val)
{
    HAL_UART_Transmit(&huart2,&val,sizeof(val),1000);
}

void SIM_pack_data(void)
{
	sprintf(S_data,"6e295ea6ff1a00%d00%dff1b 6e7f5ea6ff1a00%d00%dff1b 70df96feff1a00%d00%d00%d00%dff1b 504dff1a00%d00%d00%d00%dff1b 662f542667094ebaff1a00%d",
	                temp/10+30,temp%10+30,humi/10+30,humi%10+30,mq2/1000+30,mq2/100+30,mq2/10+30,mq2%10+30,
	                mq135/1000+30,mq135/100+30,mq135/10+30,mq135%10+30,people+30);
}

void SIM_Send_SIM(void)
{
	SIM_Send_str("AT\r\n");
	HAL_Delay(500);
	SIM_Send_str("AT+CMGF=1\r\n");
	HAL_Delay(500);
	SIM_Send_str("AT+CSCS=\"UCS2\"\r\n");
	HAL_Delay(500);
	SIM_Send_str("AT+CSCS?\r\n");
	HAL_Delay(500);
	SIM_Send_str("AT+CSMP=17,167,0,25\r\n");
	HAL_Delay(500);
	SIM_Send_str("AT+CMGS=\"00310037003600330030003500340034003500390037\"\r\n");//接受短信号码，在接受号码的每一个数字前加上003
	HAL_Delay(500);
	SIM_pack_data();
	SIM_Send_str(S_data); //温度
	HAL_Delay(500);
	SIM_Send_val(0x1a);
	HAL_Delay(3000);
	
}
