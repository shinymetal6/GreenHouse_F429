/*
 * greenhouse.c
 *
 *  Created on: Aug 25, 2021
 *      Author: fil
 */

#include "main.h"
#include "greenhouse.h"
#include <stdio.h>

DHT_DataTypedef DHT_Data;
uint8_t lcd_string[32];
uint8_t	outbit;
#define	FAN1		0x01
#define	FAN2		0x02
#define	HEATER		0x04
#define	WATERPUMP	0x08
#define	LIGHT		0x10
/*
#define FAN1_Pin GPIO_PIN_11
#define FAN1_GPIO_Port GPIOC
#define FAN2_Pin GPIO_PIN_12
#define FAN2_GPIO_Port GPIOC
#define HEATER_Pin GPIO_PIN_2
#define HEATER_GPIO_Port GPIOD
#define WATERPUMP_Pin GPIO_PIN_4
#define WATERPUMP_GPIO_Port GPIOD
*/
IconsTypedef	Icons[4] =
{
		{
				{(uint8_t *)fan48_idle ,
				(uint8_t *)fan48_active1 ,
				(uint8_t *)fan48_active2 },
				0,
				15 , 272,
				90 , 70 ,

		},
		{
				{(uint8_t *)heater48_idle ,
				(uint8_t *)heater48_active1 ,
				(uint8_t *)heater48_active2 },
				0,
				70 , 272,
				25 , 30,
		},
		{
				{(uint8_t *)waterpump48_idle ,
				(uint8_t *)waterpump48_active1 ,
				(uint8_t *)waterpump48_active2 },
				0,
				125 , 272,
				70 , 90
		},
		{
				{(uint8_t *)light48_idle ,
				(uint8_t *)light48_active1 ,
				(uint8_t *)light48_active2 },
				0,
				180 , 272,
				120 , 190,
		},

};
uint8_t	active=0;
void gotit(void)
{
	active++;
}
void setIcon(uint8_t icon_index , uint8_t state)
{
	Icons[icon_index].state = state;
	if (( icon_index == ICON_HEATER ) && (state == ICON_STATE_ACTIVE2))
		gotit();
	BSP_LCD_DrawBitmap(Icons[icon_index].xpos, Icons[icon_index].ypos, Icons[icon_index].icon_data[state]);
}

void GreenH_Init(void)
{
uint32_t x,y;

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER,LCD_FRAME_BUFFER);
	BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER,LCD_FRAME_BUFFER);
	BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_DisplayOn();
	BSP_LCD_SetFont(&Font20);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAt(0,0,(uint8_t *)"GreenHouse 1.0",CENTER_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetFont(&Font12);
	HAL_TIM_Base_Start(&DHT11_TIMER);
	HAL_TIM_Base_Start_IT(&INTERVAL_TIMER);
	DHT_Data.dht_reads = 0;
	setIcon(ICON_FAN,ICON_STATE_IDLE);
	setIcon(ICON_HEATER,ICON_STATE_IDLE);
	setIcon(ICON_WATERPUMP,ICON_STATE_IDLE);
	setIcon(ICON_LIGHT,ICON_STATE_IDLE);

	uint32_t	*ptr=(uint32_t	*)(LCD_FRAME_BUFFER+240*480);
	for(x=0;x<100;x++)
	{
		for(y=0;y<240;y++)
			*ptr++ = 0xffff0000;
	}
}

static void GreenH_ErrorAcqData(void)
{
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_DisplayStringAt(105,30,(uint8_t *)"Failed",LEFT_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
}

static void GreenH_DisplayAcqData(void)
{
	sprintf((char *)lcd_string,"T : %d.%dC",DHT_Data.TemperatureH,DHT_Data.TemperatureL);
	BSP_LCD_DisplayStringAt(0,15,lcd_string,LEFT_MODE);
	sprintf((char *)lcd_string,"H : %d%%     ",DHT_Data.HumidityH);
	BSP_LCD_DisplayStringAt(0,25,lcd_string,LEFT_MODE);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
}

static void GreenH_DisplayOutData(void)
{

}

#define	TEMP_LO			20
#define	TEMP_HI			20
#define	HUMIDITY_LO		60
#define	HUMIDITY_HI		80

void out_pid (void)
{
	if ( DHT_Data.TemperatureH < Icons[ICON_HEATER].on_value )
	{
		if (Icons[ICON_HEATER].state == ICON_STATE_IDLE )
			setIcon(ICON_HEATER,ICON_STATE_ACTIVE1);
	}
	else
	{
		if ( DHT_Data.TemperatureH > Icons[ICON_HEATER].off_value )
		{
			if (Icons[ICON_HEATER].state != ICON_STATE_IDLE )
				setIcon(ICON_HEATER,ICON_STATE_IDLE);
		}
	}

	if ( DHT_Data.HumidityH > Icons[ICON_WATERPUMP].off_value )
	{
		if (Icons[ICON_WATERPUMP].state != ICON_STATE_IDLE )
		{
			setIcon(ICON_WATERPUMP,ICON_STATE_IDLE);
		}
	}
	else
	{
		if ( DHT_Data.HumidityH < Icons[ICON_WATERPUMP].on_value )
		{
			if (Icons[ICON_WATERPUMP].state == ICON_STATE_IDLE )
			{
				setIcon(ICON_WATERPUMP,ICON_STATE_ACTIVE1);
			}
		}
	}
	if (( DHT_Data.HumidityH > Icons[ICON_FAN].on_value ) && ( DHT_Data.HumidityH < Icons[ICON_FAN].off_value  ))
		setIcon(ICON_FAN,ICON_STATE_IDLE);
}

void roll_from_active_icons(void)
{
uint8_t	i;

	for(i=ICON_FAN ; i <= ICON_LIGHT ; i++ )
	{
		if ( Icons[i].state == ICON_STATE_ACTIVE1)
			Icons[i].state = ICON_STATE_ACTIVE2;
		else if ( Icons[i].state == ICON_STATE_ACTIVE2)
			Icons[i].state = ICON_STATE_ACTIVE1;
		else
			Icons[i].state = ICON_STATE_IDLE;
		setIcon(i,Icons[i].state);
	}
}

void GreenH_main_loop(void)
{
uint8_t	DataValid = 1;
	if ( 	DHT_Data.interval_tick != 0 )
	{
		roll_from_active_icons();
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		HAL_GPIO_WritePin(LD4_GPIO_Port,LD4_Pin,GPIO_PIN_RESET);
		DHT_Data.interval_counter ++;
		switch ( DHT_Data.interval_counter )
		{
		case 2:	BSP_LCD_DisplayStringAt(105,30,(uint8_t *)"                ",LEFT_MODE); break;
		case 8 :
			HAL_GPIO_WritePin(LD4_GPIO_Port,LD4_Pin,GPIO_PIN_SET);
			if ( DHT_StartAcquisition(&DHT_Data) == 1 )
			{
				GreenH_ErrorAcqData();
				DHT_Data.interval_counter = 0;
			}
			break;
		case 9 :
			DataValid = DHT_GetData(&DHT_Data);
			if ( DataValid == 0 )
			{
				GreenH_DisplayAcqData();
				out_pid();
				DHT_Data.dht_reads++;
			}
			DHT_Data.interval_counter = 0;
			break;
		}
		DHT_Data.interval_tick = 0;
	}
}
