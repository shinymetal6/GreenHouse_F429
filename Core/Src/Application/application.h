/*
 * application.h
 *
 *  Created on: Aug 25, 2021
 *      Author: fil
 */

#ifndef SRC_APPLICATION_APPLICATION_H_
#define SRC_APPLICATION_APPLICATION_H_


extern const uint8_t fan48_active1[];
extern const uint8_t fan48_active2[];
extern const uint8_t fan48_idle[];
extern const uint8_t heater48_active1[];
extern const uint8_t heater48_active2[];
extern const uint8_t heater48_idle[];
extern const uint8_t waterpump48_active1[];
extern const uint8_t waterpump48_active2[];
extern const uint8_t waterpump48_idle[];
extern const uint8_t light48_active1[];
extern const uint8_t light48_active2[];
extern const uint8_t light48_idle[];


extern	void GreenH_Init(void);
extern	void GreenH_main_loop(void);


#endif /* SRC_APPLICATION_APPLICATION_H_ */
