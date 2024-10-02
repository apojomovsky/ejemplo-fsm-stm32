/*
 * edge_fsm.c
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#include "edge_fsm.h"

// Condition functions for edge detection FSM
static int is_rising_edge(void *context) {
    EdgeDetector *edge_detector = (EdgeDetector *)context;
    return get_debounced_switch_state(edge_detector->switch_instance) == GPIO_PIN_SET;
}

static int is_falling_edge(void *context) {
    EdgeDetector *edge_detector = (EdgeDetector *)context;
    return get_debounced_switch_state(edge_detector->switch_instance) == GPIO_PIN_RESET;
}

// Transition arrays for each state
static Transition NoEdgeTransitions[] = {
    {is_rising_edge, RISING_EDGE},
    {is_falling_edge, FALLING_EDGE}
};

static Transition RisingEdgeTransitions[] = {
    {is_falling_edge, NO_EDGE}  // Return to NO_EDGE after acknowledging rising edge
};

static Transition FallingEdgeTransitions[] = {
    {is_rising_edge, NO_EDGE}  // Return to NO_EDGE after acknowledging falling edge
};

// FSM states
static FSMState EdgeFSMStates[] = {
    {NoEdgeTransitions, 2, NULL},            // NO_EDGE state
    {RisingEdgeTransitions, 1, NULL},        // RISING_EDGE state
    {FallingEdgeTransitions, 1, NULL}        // FALLING_EDGE state
};

// Initialize the edge detector
void edge_detector_init(EdgeDetector *edge_detector, DebouncedSwitch *switch_instance) {
    fsm_init(&edge_detector->fsm, EdgeFSMStates, NO_EDGE, edge_detector);  // Start in NO_EDGE state
    edge_detector->switch_instance = switch_instance;
}

// Update the edge detector FSM
void edge_detector_update(EdgeDetector *edge_detector) {
    fsm_update(&edge_detector->fsm);
}

// Get the last detected edge based on the current detector state
EdgeFSMState get_edge_detector_edge(EdgeDetector *edge_detector) {
    return (EdgeFSMState)edge_detector->fsm.currentState;
}
