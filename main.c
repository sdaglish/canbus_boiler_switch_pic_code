/*
 * File:   main.c
 * Author: scdag
 *
 * Created on 04 October 2019, 15:36
 */


// TODO Put the config bits into their own file 

// Configuration bits: selected in the GUI

// FSEC
#pragma config BWRP = OFF    //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED    //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED    //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF    //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED    //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF    //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = FRCPLL    //Oscillator Source Selection->Fast RC Oscillator with divide-by-N with PLL module (FRCPLL) 
#pragma config PLLMODE = PLL4X    //PLL Mode Selection->4x PLL selected
#pragma config IESO = ON    //Two-speed Oscillator Start-up Enable bit->Start up device with FRC, then switch to user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE    //Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFCN = OFF    //OSC2 Pin Function bit->OSC2 is clock output
#pragma config SOSCSEL = OFF    //SOSC Power Selection Configuration bits->Digital (SCLKI) mode
#pragma config PLLSS = PLL_FRC    //PLL Secondary Selection Configuration bit->PLL is fed by the on-chip Fast RC (FRC) oscillator
#pragma config IOL1WAY = ON    //Peripheral pin select configuration bit->Allow only one reconfiguration
#pragma config FCKSM = CSDCMD    //Clock Switching Mode bits->Both Clock switching and Fail-safe Clock Monitor are disabled

// FWDT
#pragma config WDTPS = PS32768    //Watchdog Timer Postscaler bits->1:32768
#pragma config FWPSA = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config FWDTEN = OFF    //Watchdog Timer Enable bits->WDT and SWDTEN disabled
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
#pragma config WDTCMX = WDTCLK    //WDT MUX Source Select bits->WDT clock source is determined by the WDTCLK Configuration bits
#pragma config WDTCLK = LPRC    //WDT Clock Source Select bits->WDT uses LPRC

// FPOR
#pragma config BOREN = ON    //Brown Out Enable bit->Brown Out Enable Bit
#pragma config LPCFG = OFF    //Low power regulator control->No Retention Sleep
#pragma config DNVPEN = ENABLE    //Downside Voltage Protection Enable bit->Downside protection enabled using ZPBOR when BOR is inactive

// FICD
#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGEC1 and PGED1
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FDEVOPT1
#pragma config ALTCMPI = DISABLE    //Alternate Comparator Input Enable bit->C1INC, C2INC, and C3INC are on their standard pin locations
#pragma config TMPRPIN = OFF    //Tamper Pin Enable bit->TMPRN pin function is disabled
#pragma config SOSCHP = ON    //SOSC High Power Enable bit (valid only when SOSCSEL = 1->Enable SOSC high power mode (default)
#pragma config ALTI2C1 = ALTI2CEN    //Alternate I2C pin Location->SDA1 and SCL1 on RB9 and RB8

#include <xc.h>
#define FCY 16000000UL

#include <xc.h>
#include "src/io_drivers/port_a_driver.h"
#include "src/io_drivers/port_b_driver.h"
#include "src/canbus/spi_driver.h"
#include "src/canbus/mcp2515_driver.h"
#include <libpic30.h>

///////////////////////////////////////////////////////////////////////////////
//
//	Outputs
//		RA0 = switch output
//
//  SPI pins
//      cs =   RB0
//      mosi = RB1
//      miso = RB2
//      sck  = RB3
//
//  Interrupt pins
//      mcpInt = RA4
//
//  Expected can message format
//      byte 0,1 = id of expected device
//      byte 2,3 = command
//                  0 = switch output
//      byte 4+  = parameters
//                  command 0: 
//                      param 0 = pin to switch
//                      param 1 = 0 or 1 (on / off)        
//

#define MAX_BUF_LEN 8

int main(void) 
{
    uint16_t receive_id = 0;
    uint8_t receive_msg_len = 0;
    uint8_t receive_msg_buf[MAX_BUF_LEN];

    // Init pins
    // TODO Init spi pins
    port_a_driver_set_pin_to_output(0);

    port_b_driver_set_pin_to_output(0);
    port_b_driver_turn_output_high(0);
    port_b_driver_set_pin_to_output(1);
    port_b_driver_set_pin_to_input(2);
    port_b_driver_set_pin_to_output(3);

    port_b_driver_set_pin_to_input(4);
    
    ANSELB = 0X00;

    __builtin_write_OSCCONL(OSCCON & 0xbf);
    // RB0 = RP0 = CS 
    _RP0R = 9; //SS1OUT;
    // RB1 = RP1 = MOSI
    _RP1R = 7; //SDO1;
    // RB2 = RP2 = MISO
    _SDI1R = 2;
    // RB3 = RP3 = SCK
    _RP3R = 8; //SCK1OUT;
    __builtin_write_OSCCONL(OSCCON | 0x40);

    // Init drivers and modules
    spi_driver_init();
    mcp2515_init();

    while(1)
    {
        if (true == port_a_driver_pin_is_low(4))
        {
            // Since only rx0 full is connected to the INT pin, I can safely presume that there is now data in the rx0 buffer
            // In later iterations, I'll be unable to assume that, so will need to check the status flag to see what the interrupt was called for
            mcp2515_driver_read_can_message(&receive_id, &receive_msg_len, receive_msg_buf);
            mcp2515_driver_clear_rx0if();
            
    }
        
    return 0;
}
