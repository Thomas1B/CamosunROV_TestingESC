/*
 * my_utils.h
 *
 *  Created on: Jul 17, 2026
 *      Author: Thomas Bourgeois
 */

#ifndef MYUTILS_MY_UTILS_H_
#define MYUTILS_MY_UTILS_H_


#include <stdint.h>


float map(float x, float in_min, float in_max, float out_min, float out_max);


float analogToVoltage(uint16_t val);

#endif /* MYUTILS_MY_UTILS_H_ */
