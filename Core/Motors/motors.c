/*
 * motors.c
 *
 *  Created on: Jul 19, 2026
 *  Author: Thomas Bourgeois
 *
 *  This software is written to control either an Afro30A or BlueRobotics Basic ESC.
 *
 *	Refer to STM32f446xx datasheet for TIM and TIM_CHANNEL.
 *
 *	Note: Static - means the function is contained only within this file.
 *		  Inline - removes overhead (only works for small functions).
 */

#include "motors.h"
#include "main.h"
#include <math.h>
#include "my_utils.h"

/*
 * @brief Converts throttle percentage to pulse width between 1100us and 1900us.
 * @param throttlePercent: throttle input, range -100% to +100%.
 * @retval Pulse width in microseconds.
 */
static inline uint32_t throttleToPulse(int8_t throttlePercent) {
	return lroundf(map(throttlePercent, -100.0f, 100.0f, 1100.0f, 1900.0f));
}

/**
 * @brief Writes a motor throttle value to a timer PWM channel.
 * @param htim Pointer to the timer handle driving the PWM output.
 * @param channel Timer channel to update.
 * @param throttlePercent Throttle value as a percentage (-100 to 100).
 */
static inline void motor_write(TIM_HandleTypeDef *htim, uint32_t channel,
		int8_t throttlePercent) {
	__HAL_TIM_SET_COMPARE(htim, channel, throttleToPulse(throttlePercent));
}

void reset_motors() {
	__HAL_TIM_MOE_ENABLE(&htim1);
	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_BREAK);

	__HAL_TIM_MOE_ENABLE(&htim8);
	__HAL_TIM_CLEAR_FLAG(&htim8, TIM_FLAG_BREAK);
}

/**
 * @brief Sets motor 1 throttle
 * @param throttlePercent Throttle value as a percentage (-100 to 100).
 */
void motor1(int8_t throttlePercent) {
	motor_write(MOTOR1_TIM, MOTOR1_CHANNEL, throttlePercent);
}

/**
 * @brief Sets motor 2 throttle
 * @param throttlePercent Throttle value as a percentage (-100 to 100).
 */
void motor2(int8_t throttlePercent) {
	motor_write(MOTOR2_TIM, MOTOR2_CHANNEL, throttlePercent);
}

/**
 * @brief Sets motor 3 throttle
 * @param throttlePercent Throttle value as a percentage (-100 to 100).
 */
void motor3(int8_t throttlePerecent) {
	motor_write(MOTOR3_TIM, MOTOR3_CHANNEL, throttlePercent);
}

/**
 * @brief Sets motor 4 throttle
 * @param throttlePercent Throttle value as a percentage (-100 to 100).
 */
void motor4(int8_t throttlePerecent) {
	motor_write(MOTOR4_TIM, MOTOR4_CHANNEL, throttlePercent);
}

/**
 * @brief Sets motor 5 throttle
 * @param throttlePercent Throttle value as a percentage (-100 to 100).
 */
void motor5(int8_t throttlePerecent) {
	motor_write(MOTOR5_TIM, MOTOR5_CHANNEL, throttlePercent);
}

/**
 * @brief Sets motor 6 throttle
 * @param throttlePercent Throttle value as a percentage (-100 to 100).
 */
void motor6(int8_t throttlePerecent) {
	motor_write(MOTOR6_TIM, MOTOR6_CHANNEL, throttlePercent);
}



