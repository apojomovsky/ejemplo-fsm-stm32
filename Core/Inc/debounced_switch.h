/*
 * debounced_switch.h
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#ifndef INC_DEBOUNCED_SWITCH_H_
#define INC_DEBOUNCED_SWITCH_H_

#include "fsm.h"
#include <main.h>
#include <stdint.h>

// Enum representing the debounced switch states
typedef enum {
    SWITCH_IDLE,
    SWITCH_PRESSED,
    SWITCH_RELEASED
} DebouncedSwitchStates;

/**
 * @brief Initializes the FSM for the debounced switch.
 * @param fsm Pointer to the FSM structure.
 */
void debounced_switch_init(FSM *fsm);

/**
 * @brief Reads the current state of the switch (raw).
 * @param GPIOx GPIO port of the switch.
 * @param GPIO_Pin GPIO pin of the switch.
 * @return 1 if the switch is pressed, 0 otherwise.
 */
int read_switch(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Sets the debounced switch output (used by FSM).
 * @param outputs The FSM state's outputs.
 * @param num_outputs The number of outputs.
 */
void set_switch_output(unsigned long *outputs, unsigned long num_outputs);

/**
 * @brief Get the GPIO pin state based on the FSM state.
 * @param fsm Pointer to the FSM structure.
 * @return GPIO_PIN_SET or GPIO_PIN_RESET based on the debounced state.
 */
GPIO_PinState get_debounced_state(FSM *fsm);

#endif /* INC_DEBOUNCED_SWITCH_H_ */
