/*
 * debounced_switch.c
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */


#include "debounced_switch.h"
#include "main.h"

// Define the outputs for each state
unsigned long SwitchOffOutputs[] = {0};  // Switch is not pressed
unsigned long SwitchOnOutputs[] = {1};   // Switch is pressed

// Define state transitions for the switch
unsigned long IdleNext[] = {SWITCH_IDLE, SWITCH_PRESSED};
unsigned long PressedNext[] = {SWITCH_RELEASED, SWITCH_PRESSED};
unsigned long ReleasedNext[] = {SWITCH_IDLE, SWITCH_PRESSED};

// FSM states for the debounced switch
const FSMState DebouncedSwitchFSM[] = {
    {SwitchOffOutputs, 1, IdleNext, 2},
    {SwitchOnOutputs, 1, PressedNext, 2},
    {SwitchOffOutputs, 1, ReleasedNext, 2}
};

// Initialize the FSM for the debounced switch
void debounced_switch_init(FSM *fsm) {
    fsm_init(fsm, DebouncedSwitchFSM, SWITCH_IDLE);
}

// Read the switch state (1 if pressed, 0 if not)
int read_switch(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET ? 1 : 0;
}

// Handle the outputs (for this example, we don't need complex output handling)
void set_switch_output(unsigned long *outputs, unsigned long num_outputs) {
    // Here we could handle multiple outputs, but for a button it's binary (pressed or not)
}

// Get the debounced GPIO state based on FSM state
GPIO_PinState get_debounced_state(FSM *fsm) {
    return (fsm->CurrentState == SWITCH_PRESSED) ? GPIO_PIN_RESET : GPIO_PIN_SET;
}

