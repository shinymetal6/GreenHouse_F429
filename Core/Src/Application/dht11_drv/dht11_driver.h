/*
 * dht11_driver.h
 *
 *  Created on: May 10, 2021
 *      Author: fil
 */

#ifndef INC_TEMPH_DHT11_DRIVER_H_
#define INC_TEMPH_DHT11_DRIVER_H_

#define	EXTI_IRQ	EXTI9_5_IRQn

typedef struct
{
	uint8_t first_irq;
	uint8_t TemperatureH,TemperatureL;
	uint8_t HumidityH,HumidityL;
	uint8_t interval_tick;
	uint8_t interval_counter;
	uint8_t dht_reads;
	uint8_t dht_done;
}DHT_DataTypedef;

extern	TIM_HandleTypeDef htim7;
extern	TIM_HandleTypeDef htim14;

#define	DELTA_TIMER		TIM7
#define	DHT11_TIMER		htim7
extern	DHT_DataTypedef DHT_Data;
#define	INTERVAL_TIMER	htim14

#define	DHT11_NUMBITS	40

#define	DIRECTION_IN	0x00
#define	DIRECTION_OUT	0x01

extern	uint8_t DHT_StartAcquisition (DHT_DataTypedef *DHT_Data);
extern	uint8_t DHT_GetData (DHT_DataTypedef *DHT_Data);

extern	void DHT_IRQ_callback(void);
extern	void IntervalTimer_IRQ_callback(void);
extern	void tim_usecdelay_callback(void);

#endif /* INC_TEMPH_DHT11_DRIVER_H_ */
