/*
 * modulo_ejemplo.c
 *
 *  Created on: [Fecha de creación]
 *      Author: [Tu nombre]
 */

#include "modulo_ejemplo.h"

/***********************************************
*        PASO 4: FUNCIONES DE TRANSICIÓN       *
* Definimos las funciones de transición que    *
* dependen de las condiciones para cambiar de  *
* estado.                                      *
***********************************************/

/**
 * @brief Condición para pasar del estado 1 al estado 2.
 */
static int condicion_estado1_a_estado2(void *context) {
    ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
    return HAL_GPIO_ReadPin(ejemplo->Port, ejemplo->Pin) == GPIO_PIN_SET;
}

/**
 * @brief Condición para pasar del estado 2 al estado 3.
 */
static int condicion_estado2_a_estado3(void *context) {
    ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
    return HAL_GPIO_ReadPin(ejemplo->Port, ejemplo->Pin) == GPIO_PIN_RESET;
}

/**
 * @brief Condición alternativa para volver del estado 2 al estado 1.
 */
static int condicion_estado2_a_estado1(void *context) {
    ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
    // Una segunda condición que, si es verdadera, devuelve al Estado 1
    return timer_has_expired(ejemplo->timer); // Ejemplo de uso de timer
}

/**
 * @brief Condición para pasar del estado 3 al estado 1.
 */
static int condicion_estado3_a_estado1(void *context) {
    ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
    return HAL_GPIO_ReadPin(ejemplo->Port, ejemplo->Pin) == GPIO_PIN_SET;
}

/***********************************************
*          PASO 5: FUNCIONES DE ACCIÓN         *
* Aquí se definen las acciones que se ejecutan *
* al entrar en un nuevo estado.                *
***********************************************/

/**
 * @brief Acción al entrar en el estado 1.
 */
void on_state_estado1(void *context) {
    ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
    // Ejemplo de acción: Cambiar el estado del pin GPIO a RESET
    HAL_GPIO_WritePin(ejemplo->Port, ejemplo->Pin, GPIO_PIN_RESET);
}

/**
 * @brief Acción al entrar en el estado 2.
 */
void on_state_estado2(void *context) {
    ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
    // Ejemplo de acción: Iniciar el timer asociado
    timer_start(ejemplo->timer, 5000); // Iniciar un timer de 5 segundos
}

/**
 * @brief Acción al entrar en el estado 3.
 */
void on_state_estado3(void *context) {
    ModuloEjemplo *ejemplo = (ModuloEjemplo *)context;
    // Ejemplo de acción: Cambiar el estado del pin GPIO a SET
    HAL_GPIO_WritePin(ejemplo->Port, ejemplo->Pin, GPIO_PIN_SET);
}

/***********************************************
*          PASO 6: DECLARAR TRANSICIONES       *
* Aquí declaramos las transiciones de estado,  *
* especificando las condiciones que hacen      *
* posible cambiar entre estados.               *
***********************************************/

// Transiciones desde ESTADO_1
static Transition Estado1Transitions[] = {
    {condicion_estado1_a_estado2, ESTADO_2},  // Transición al estado 2
};

// Transiciones desde ESTADO_2 (2 transiciones)
static Transition Estado2Transitions[] = {
    {condicion_estado2_a_estado3, ESTADO_3},  // Transición al estado 3
    {condicion_estado2_a_estado1, ESTADO_1}   // Alternativa: volver al estado 1
};

// Transiciones desde ESTADO_3
static Transition Estado3Transitions[] = {
    {condicion_estado3_a_estado1, ESTADO_1}   // Transición al estado 1
};

/***********************************************
*           PASO 7: ENSAMBLAR EL MÓDULO        *
* Aquí se ensamblan los estados, las           *
* transiciones y las acciones correspondientes.*
***********************************************/

static FSMState ModuloEjemploEstados[] = {
    {Estado1Transitions, 1, on_state_estado1},  // Estado 1: tiene 1 transición
    {Estado2Transitions, 2, on_state_estado2},  // Estado 2: tiene 2 transiciones
    {Estado3Transitions, 1, on_state_estado3}   // Estado 3: tiene 1 transición
};

/***********************************************
*         PASO 8: INICIALIZAR EL MÓDULO        *
* Aquí inicializamos el módulo con el estado   *
* inicial y el contexto adecuado.              *
***********************************************/

/**
 * @brief Inicializa el módulo.
 */
void modulo_ejemplo_init(ModuloEjemplo *ejemplo, GPIO_TypeDef *Port, uint16_t Pin) {
    // Iniciar la FSM en el estado 1
    fsm_init(&ejemplo->fsm, ModuloEjemploEstados, ESTADO_1, ejemplo);
    // Inicializar el puerto y pin GPIO
    ejemplo->Port = Port;
    ejemplo->Pin = Pin;
    // Inicializar otros recursos (si es necesario)
}

/***********************************************
*         PASO 9: ACTUALIZAR EL MÓDULO         *
* Esta función se llama en cada ciclo para     *
* verificar las transiciones y actualizar el   *
* estado actual si corresponde.                *
***********************************************/

/**
 * @brief Actualiza el módulo.
 */
void modulo_ejemplo_update(ModuloEjemplo *ejemplo) {
    fsm_update(&ejemplo->fsm);  // Llamar a fsm_update para gestionar las transiciones
}
