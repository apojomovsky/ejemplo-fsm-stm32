/*
 * edge_fsm.c
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#include "edge_fsm.h"

// Condition functions for edge detection FSM
static int is_high(void *context) {
    EdgeDetector *edge_detector = (EdgeDetector *)context;
    return get_debounced_switch_state(edge_detector->switch_instance) == GPIO_PIN_SET;
}

static int is_low(void *context) {
    EdgeDetector *edge_detector = (EdgeDetector *)context;
    return get_debounced_switch_state(edge_detector->switch_instance) == GPIO_PIN_RESET;
}

// Transition arrays for each state
static Transition IdleHighTransitions[] = {
    {is_low, FALLING_EDGE}   // Transition to FALLING_EDGE on low input
};

static Transition IdleLowTransitions[] = {
    {is_high, RISING_EDGE}   // Transition to RISING_EDGE on high input
};

static Transition RisingEdgeTransitions[] = {
    {is_high, IDLE_HIGH},    // Transition to IDLE_HIGH on high input
    {is_low, FALLING_EDGE}   // Transition to FALLING_EDGE on low input
};

static Transition FallingEdgeTransitions[] = {
    {is_low, IDLE_LOW},      // Transition to IDLE_LOW on low input
    {is_high, RISING_EDGE}   // Transition to RISING_EDGE on high input
};

// FSM states
static FSMState EdgeFSMStates[] = {
    {IdleHighTransitions, 1, NULL},         // IDLE_HIGH state
    {IdleLowTransitions, 1, NULL},          // IDLE_LOW state
    {RisingEdgeTransitions, 2, NULL},       // RISING_EDGE state
    {FallingEdgeTransitions, 2, NULL}       // FALLING_EDGE state
};

// Initialize the edge detector
void edge_detector_init(EdgeDetector *edge_detector, DebouncedSwitch *switch_instance) {
    fsm_init(&edge_detector->fsm, EdgeFSMStates, IDLE_LOW, edge_detector);  // Start in IDLE_LOW
    edge_detector->switch_instance = switch_instance;
}

// Update the edge detector FSM
void edge_detector_update(EdgeDetector *edge_detector) {
    fsm_update(&edge_detector->fsm);
}

// Get the current state based on the edge detector FSM state
EdgeFSMState get_edge_detector_state(EdgeDetector *edge_detector) {
    return (EdgeFSMState)edge_detector->fsm.currentState;
}
