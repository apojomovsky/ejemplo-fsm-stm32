/*
 * fsm.c
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#include "fsm.h"

/**
 * @brief Initializes the FSM with given states, initial state, and context.
 */
void fsm_init(FSM *fsm, const FSMState *states, unsigned long initial_state, void *context) {
    fsm->states = states;
    fsm->currentState = initial_state;
    fsm->context = context;
}

/**
 * @brief Updates the FSM based on the conditions of the current state.
 */
void fsm_update(FSM *fsm) {
    FSMState currentState = fsm->states[fsm->currentState];

    // Check all transitions
    for (unsigned long i = 0; i < currentState.numTransitions; i++) {
        if (currentState.transitions[i].condition(fsm->context)) {  // Pass the context to the condition function
            fsm->currentState = currentState.transitions[i].nextState;  // Transition to the corresponding next state
            if (currentState.action) {
                currentState.action(fsm->context);  // Pass the context to the action function
            }
            return;  // Exit after the first matched condition
        }
    }
}
