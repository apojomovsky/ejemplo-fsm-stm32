/*
 * fsm.c
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#include "fsm.h"

/**
 * @brief Initializes the FSM.
 */
void fsm_init(FSM *fsm, const FSMState *states, unsigned long initial_state) {
    fsm->States = states;
    fsm->CurrentState = initial_state;
}

/**
 * @brief Updates the FSM based on the input.
 */
void fsm_update(FSM *fsm, unsigned long input, void (*set_output)(unsigned long *, unsigned long)) {
    // Update the current state based on input and set the outputs
    if (input < fsm->States[fsm->CurrentState].NumInputs) {
        fsm->CurrentState = fsm->States[fsm->CurrentState].Next[input];
        set_output(fsm->States[fsm->CurrentState].Outputs, fsm->States[fsm->CurrentState].NumOutputs);
    }
}
