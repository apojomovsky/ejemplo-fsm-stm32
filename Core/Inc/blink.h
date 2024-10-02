/*
 * blink.h
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#ifndef INC_BLINK_H_
#define INC_BLINK_H_

#include "fsm.h"
#include "timer.h"

typedef enum {
    BLINK_IDLE,
    BLINK_ON,
    BLINK_OFF
} BlinkFSMState;

// Blink FSM structure
typedef struct {
    FSM fsm;               // FSM instance
    Timer blink_timer;     // Timer for blinking delay
    GPIO_TypeDef *GPIOx;   // GPIO port for the LED
    uint16_t GPIO_Pin;     // GPIO pin for the LED
} Blink;

/**
 * @brief Initializes the Blink FSM.
 *
 * @param blink Pointer to Blink structure.
 * @param GPIOx GPIO port of the LED.
 * @param GPIO_Pin GPIO pin of the LED.
 */
void blink_init(Blink *blink, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Updates the Blink FSM.
 *
 * @param blink Pointer to Blink structure.
 */
void blink_update(Blink *blink);

/**
 * @brief Sets a new delay duration for the blinking without resetting the timer.
 *
 * @param blink Pointer to Blink structure.
 * @param new_delay_ms New delay in milliseconds.
 */
void blink_set_delay(Blink *blink, uint32_t new_delay_ms);

#endif /* INC_BLINK_H_ */
