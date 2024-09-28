/*
 * fsm.h
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

/**
 * @brief FSM State structure
 * This structure represents a state in the FSM, its outputs, and transitions.
 */
typedef struct {
    unsigned long *Outputs;       // Pointer to an array of output values
    unsigned long NumOutputs;     // Number of outputs for this state
    unsigned long *Next;          // Pointer to next states array based on input
    unsigned long NumInputs;      // Number of possible inputs
} FSMState;

/**
 * @brief FSM control structure
 * Contains the current state information for the FSM.
 */
typedef struct {
    const FSMState *States;       // Pointer to FSM states array
    unsigned long CurrentState;   // Current state index
} FSM;

/**
 * @brief Initializes the FSM.
 *
 * @param fsm Pointer to the FSM structure.
 * @param states Pointer to the array of FSMState.
 * @param initial_state The initial state index.
 */
void fsm_init(FSM *fsm, const FSMState *states, unsigned long initial_state);

/**
 * @brief Updates the FSM based on input and sets the outputs.
 *
 * @param fsm Pointer to FSM structure.
 * @param input Current input value.
 * @param set_output Function pointer for setting the outputs.
 */
void fsm_update(FSM *fsm, unsigned long input, void (*set_output)(unsigned long *, unsigned long));

#endif /* INC_FSM_H_ */
