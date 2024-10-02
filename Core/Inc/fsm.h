/*
 * fsm.h
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include <stdint.h>

// Define a function pointer type for condition functions with context
typedef int (*ConditionFunc)(void *context);

// Define a function pointer type for action functions with context
typedef void (*ActionFunc)(void *context);

// Define a Transition structure to pair conditions with next states
typedef struct {
    ConditionFunc condition;  // Condition function
    unsigned long nextState;  // Corresponding next state if the condition is true
} Transition;

typedef struct {
    Transition *transitions;  // Array of transitions (condition -> next state)
    unsigned long numTransitions; // Number of transitions
    ActionFunc action;        // Action to be executed on entering the state
} FSMState;

typedef struct {
    const FSMState *states;
    unsigned long currentState;
    void *context;  // Context to pass to condition and action functions
} FSM;

/**
 * @brief Initializes the FSM.
 *
 * @param fsm Pointer to FSM structure.
 * @param states Array of FSMState that defines the FSM.
 * @param initial_state The starting state of the FSM.
 * @param context A pointer to the FSM's context (e.g., DebouncedSwitch instance).
 */
void fsm_init(FSM *fsm, const FSMState *states, unsigned long initial_state, void *context);

/**
 * @brief Updates the FSM based on evaluated conditions.
 *
 * @param fsm Pointer to FSM structure.
 */
void fsm_update(FSM *fsm);

#endif /* INC_FSM_H_ */
