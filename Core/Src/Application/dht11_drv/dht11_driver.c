/*
 * dht11_driver.c
 *
 *  Created on: May 10, 2021
 *      Author: fil
 */
#include "main.h"

uint32_t			moder,data_index;
uint32_t			capture_data[DHT11_NUMBITS+4];
DHT_DataTypedef 	DHT_Data;
uint8_t				received=0;

static void delay_us (uint32_t us)
{
	DELTA_TIMER->CNT = 0;
	while (DELTA_TIMER->CNT < us );
}

static uint8_t DHT_decode(void)
{
uint8_t		i , byte_idx = 0 , bit_idx = 8 ;
uint8_t		rec_data[5];
uint16_t 	SUM;

	for(i=0;i<5; i++)
		rec_data[i] = 0;

	for(i=1;i<DHT11_NUMBITS+1; i++)
	{
		if ( capture_data[i] > 100 )
			rec_data[byte_idx] |= 1 << (bit_idx-1);
		bit_idx --;
		if ( bit_idx == 0 )
		{
			bit_idx = 8;
			byte_idx++;
			if ( byte_idx == 5 )
			{
				SUM = rec_data[0]+rec_data[1]+rec_data[2]+rec_data[3];
				if ( SUM != rec_data[4])
					return 1;
				DHT_Data.TemperatureH = rec_data[2];
				DHT_Data.TemperatureL = rec_data[3];
				DHT_Data.HumidityH = rec_data[0];
				DHT_Data.HumidityL = rec_data[1];
				return 0;
			}
		}
	}
	return 1;
}

static uint8_t DHT_Start (void)
{
uint8_t R0 = 0 , R1 = 0 , i;
GPIO_InitTypeDef GPIO_InitStruct = {0};

	for(i=0;i<DHT11_NUMBITS+4;i++)
		capture_data[i] = 0;
	DELTA_TIMER->CNT = 0;
	data_index = received = 0;
	/*Configure GPIO pin : DHT_Pin out */
	HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = DHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);

	HAL_Delay(18);

	/*Configure GPIO pin : DHT_Pin in */
	HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = DHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);

	delay_us (50);
	R0 =  HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin);
	delay_us (60);
	R1 = HAL_GPIO_ReadPin (DHT_GPIO_Port, DHT_Pin);
	HAL_GPIO_WritePin(DHT_GPIO_Port, DHT_Pin, GPIO_PIN_SET);

	delay_us (50);
	/*Configure GPIO pin : DHT_Pin exti */
	GPIO_InitStruct.Pin = DHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT_GPIO_Port, &GPIO_InitStruct);

	return ~R0 & R1;
}

uint8_t DHT_StartAcquisition (DHT_DataTypedef *DHT_Data)
{
    if ( DHT_Start () != 0 )
    {
		DHT_Data->first_irq = 0;
		NVIC_EnableIRQ(EXTI_IRQ);
		return 0;
    }
    return 1;
}

uint8_t DHT_GetData (DHT_DataTypedef *DHT_Data)
{
	//DHT_Data->dht_done = 1;
//	NVIC_DisableIRQ(EXTI_IRQ);
	return DHT_decode();
}

void DHT_IRQ_callback(void)
{
	if ( DHT_Data.first_irq != 0 )
	{
		DHT_Data.first_irq--;
		DELTA_TIMER->CNT = 0;
		data_index = received = 0;
		return;
	}
	capture_data[data_index] = DELTA_TIMER->CNT;
	DELTA_TIMER->CNT = 0;
	data_index++;
	if ( data_index >= DHT11_NUMBITS+1)
	{
		NVIC_DisableIRQ(EXTI_IRQ);
		DHT_Data.dht_done = 1;
	}
}

void IntervalTimer_IRQ_callback(void)
{
	DHT_Data.interval_tick ++;
}
