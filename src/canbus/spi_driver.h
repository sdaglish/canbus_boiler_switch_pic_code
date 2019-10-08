/**
 * @file        spi_driver.h
 * @author      Steven Daglish
 * @brief       Header file for spi_driver
 * @version     0.1
 * @date        05 October 2019
 *
 */

#include <xc.h>
#include <stdint.h>
#include "../io_drivers/port_b_driver.h"

void spi_driver_init(void);
uint8_t spi_driver_exchange(uint8_t data);
void spi_driver_cs_low(void);
void spi_driver_cs_high(void);

