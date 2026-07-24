/*
 * motors.h
 *
 *  Created on: Jul 19, 2026
 *      Author: thoma
 */

#ifndef MOTORS_MOTORS_H_
#define MOTORS_MOTORS_H_

#include <stdint.h>
#include "main.h"          /* gives us TIM_HandleTypeDef, TIM_CHANNEL_x */

extern TIM_HandleTypeDef htim1;   /* the real variable lives in main.c */

#define MOTOR1_TIM      &htim1
#define MOTOR1_CHANNEL  TIM_CHANNEL_1
#define MOTOR2_TIM      &htim1
#define MOTOR2_CHANNEL  TIM_CHANNEL_2
#define MOTOR3_TIM      &htim1
#define MOTOR3_CHANNEL  TIM_CHANNEL_3
#define MOTOR4_TIM      &htim1
#define MOTOR4_CHANNEL  TIM_CHANNEL_4
#define MOTOR5_TIM      &htim8
#define MOTOR5_CHANNEL  TIM_CHANNEL_1
#define MOTOR6_TIM      &htim8
#define MOTOR6_CHANNEL  TIM_CHANNEL_2

void motor1(int8_t throttlePercent);
void motor2(int8_t throttlePercent);

void reset_motors();

#endif
