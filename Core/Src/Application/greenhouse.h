/*
 * greenhouse.h
 *
 *  Created on: Aug 25, 2021
 *      Author: fil
 */

#ifndef SRC_APPLICATION_GREENHOUSE_H_
#define SRC_APPLICATION_GREENHOUSE_H_

typedef struct _IconsTypedef
{
	uint8_t 		*icon_data[3];
	uint8_t 		state;
	uint16_t		xpos,ypos;
	uint16_t		on_value,off_value;
}IconsTypedef;

#define	ICON_STATE_IDLE		0
#define	ICON_STATE_ACTIVE1	1
#define	ICON_STATE_ACTIVE2	2

#define	ICON_FAN		0
#define	ICON_HEATER		1
#define	ICON_WATERPUMP	2
#define	ICON_LIGHT		3

#endif /* SRC_APPLICATION_GREENHOUSE_H_ */
