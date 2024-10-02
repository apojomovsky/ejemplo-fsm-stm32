/*
 * edge_fsm.h
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#ifndef INC_EDGE_FSM_H_
#define INC_EDGE_FSM_H_

#include "fsm.h"
#include "debounced_switch.h"

typedef enum {
    NO_EDGE,         // No edge detected
    RISING_EDGE,     // Rising edge detected
    FALLING_EDGE     // Falling edge detected
} EdgeFSMState;

typedef struct {
    FSM fsm;               // FSM instance for edge detection
    DebouncedSwitch *switch_instance;  // Reference to the debounced switch
} EdgeDetector;

/**
 * @brief Initializes the edge detector FSM.
 *
 * @param edge_detector Pointer to the EdgeDetector structure.
 * @param switch_instance Pointer to the debounced switch instance.
 */
void edge_detector_init(EdgeDetector *edge_detector, DebouncedSwitch *switch_instance);

/**
 * @brief Updates the edge detector FSM.
 *
 * @param edge_detector Pointer to the EdgeDetector structure.
 */
void edge_detector_update(EdgeDetector *edge_detector);

/**
 * @brief Gets the last detected edge.
 *
 * @param edge_detector Pointer to the EdgeDetector structure.
 * @return EdgeFSMState The last detected edge (RISING_EDGE, FALLING_EDGE, NO_EDGE).
 */
EdgeFSMState get_edge_detector_edge(EdgeDetector *edge_detector);

#endif /* INC_EDGE_FSM_H_ */
