/**
 * @file        spi_driver.c
 * @author      Steven Daglish
 * @brief       Source file for spi_driver for pic24fj256ga702
 * @version     0.1
 * @date        05 October 2019
 *
 * @note        The spi pins (including PPS) should already be setup before calling
 *              spi_driver_init
 *
 * @note        No interrupts are currently used, so exchanges are blocking
 *
 * @note        cs_high and cs_low will need to be changed for each appilcation
 *
 */

#include "spi_driver.h"

void spi_driver_init(void)
{
    // TODO Check if I need to init the spi mobule before setting the other registers
    // At the moment I don't think that I do, but I have been burned with that
    // one before
    SPI1CON1Lbits.SPIEN = 0;
    SPI1CON1Lbits.MSTEN = 1;
    SPI1CON1Lbits.CKE = 1;
    
    // @ baud-rate = 500kHz
    SPI1BRGL = 0x000F;
//    SPI1BRGL = 0x0000;

    SPI1CON1Lbits.SPIEN = 1;
}

uint8_t spi_driver_exchange(uint8_t data)
{
   while (1 == SPI1STATLbits.SPIBUSY)
   {
       // Waiting for spi module to be idle
   }
   while (1 == SPI1STATLbits.SPITBF)
   {
       // Double checking, waiting for the tx buffer to be empty
   }

   SPI1BUFL = data;

   while (1 == SPI1STATLbits.SPIBUSY)
   {
       // Waiting for spi module to be idle
   }
   while (1 == SPI1STATLbits.SPITBF)
   {
       // Double checking, waiting for the tx buffer to be empty
   }
   while (0 == SPI1STATLbits.SPIRBF)
   {
       // Waiting for the rx buffer to be receive data
   }
   
   return SPI1BUFL;
}

void spi_driver_cs_low(void)
{
    port_a_driver_turn_output_low(2);
}

void spi_driver_cs_high(void)
{
    port_a_driver_turn_output_high(2);
}


