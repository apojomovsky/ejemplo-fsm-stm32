/*
 * timer.c
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */


#include "main.h"
#include "timer.h"

/**
 * @brief Starts a timer for the specified duration in milliseconds.
 */
void timer_start(Timer *timer, uint32_t duration_ms) {
    timer->start_tick = HAL_GetTick();    // Store the current tick
    timer->timeout_duration = duration_ms; // Set the timeout duration
}

/**
 * @brief Checks if the timer has expired.
 *
 * @return 1 if the timer has expired, 0 if not.
 */
int timer_has_expired(Timer *timer) {
    return (HAL_GetTick() - timer->start_tick >= timer->timeout_duration);
}
