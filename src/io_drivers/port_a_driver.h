/**
 * @file            port_a_driver.h
 * @author          Steven Daglish
 * @brief           Module to control the port_a_driver
 * @version         1.0
 * @date            22 August 2019
 *
 * @note            This will only work on PICs with 16bit IO registers. It was
 * developed for use with PIC24F range
 *
 * @todo            Add digital input control
 * @todo            Add analgue input control
 */

#ifndef _PORT_A_DRIVER_H
#define _PORT_A_DRIVER_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "io_driver.h"

void port_a_driver_set_all_to_output(void);

void port_a_driver_set_all_to_input(void);

uint8_t port_a_driver_set_pin_to_output(uint8_t pin);

uint8_t port_a_driver_set_pin_to_input(uint8_t pin);

void port_a_driver_turn_all_outputs_high(void);

void port_a_driver_turn_all_outputs_low(void);

uint8_t port_a_driver_turn_output_high(uint8_t pin);

uint8_t port_a_driver_turn_output_low(uint8_t pin);

uint8_t port_a_driver_pin_is_low(uint8_t pin);

#endif // _PORT_A_DRIVER_H
