/*
 * blink.c
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#include "blink.h"
#include "timer.h"

// Condition functions for state transitions
static int is_button_pressed(void *context) {
    Blink *blink = (Blink *)context;
    return HAL_GPIO_ReadPin(blink->GPIOx, blink->GPIO_Pin) == GPIO_PIN_SET;
}

static int has_timer_expired(void *context) {
    Blink *blink = (Blink *)context;
    return timer_has_expired(&blink->blink_timer);
}

// Action functions
static void turn_led_on(void *context) {
    Blink *blink = (Blink *)context;
    HAL_GPIO_WritePin(blink->GPIOx, blink->GPIO_Pin, GPIO_PIN_SET);
    timer_start(&blink->blink_timer, blink->blink_timer.timeout_duration);  // Start the timer with the current duration
}

static void turn_led_off(void *context) {
    Blink *blink = (Blink *)context;
    HAL_GPIO_WritePin(blink->GPIOx, blink->GPIO_Pin, GPIO_PIN_RESET);
    timer_start(&blink->blink_timer, blink->blink_timer.timeout_duration);  // Start the timer with the current duration
}

// Transition arrays for each state
static Transition IdleTransitions[] = {
    {is_button_pressed, BLINK_ON}
};

static Transition OnTransitions[] = {
    {has_timer_expired, BLINK_OFF}
};

static Transition OffTransitions[] = {
    {has_timer_expired, BLINK_ON}
};

// FSM states with actions
static FSMState BlinkFSM[] = {
    {IdleTransitions, 1, NULL},           // BLINK_IDLE state
    {OnTransitions, 1, turn_led_on},      // BLINK_ON state, turns on LED
    {OffTransitions, 1, turn_led_off}     // BLINK_OFF state, turns off LED
};

// Initialize the blink FSM
void blink_init(Blink *blink, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    fsm_init(&blink->fsm, BlinkFSM, BLINK_IDLE, blink);  // Pass context as the blink instance
    blink->GPIOx = GPIOx;
    blink->GPIO_Pin = GPIO_Pin;
    timer_start(&blink->blink_timer, 1000);  // Set an initial 1-second delay
}

// Update the blink FSM
void blink_update(Blink *blink) {
    fsm_update(&blink->fsm);
}

// Set the delay for the blink FSM
void blink_set_delay(Blink *blink, uint32_t new_delay_ms) {
    timer_update_duration(&blink->blink_timer, new_delay_ms);
}
