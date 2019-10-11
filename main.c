/*
 * File:   main.c
 * Author: scdag
 *
 * Created on 04 October 2019, 15:36
 *
 * @version     1.2
 */


// TODO Put the config bits into their own file 

#define FCY 16000000UL


#include <xc.h>
#include "src/io_drivers/port_a_driver.h"
#include "src/io_drivers/port_b_driver.h"
#include "src/canbus/spi_driver.h"
#include "src/canbus/mcp2515_driver.h"
#include "src/config.h"
#include <libpic30.h>

///////////////////////////////////////////////////////////////////////////////
//
//	Outputs
//		RA0 = switch output
//		    1 = switch off
//		    0 = switch on
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
//                  1 = boiler switch
//      byte 2,3 = command
//                  0 = switch output
//      byte 4+  = parameters
//                  command 0: 
//                      param 0 = pin to switch
//                      param 1 = 0 or 1 (off / on)        
//

#define MAX_BUF_LEN 8
#define DEVICE_ID   1

int main(void) 
{
    uint8_t receive_id = 0;
    uint16_t expected_id = 0;
    int16_t receive_command = 0;
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

    port_a_driver_set_pin_to_input(4);
    
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
            
            expected_id = ((receive_msg_buf[1] << 8) + receive_msg_buf[0]);
            receive_command = ((receive_msg_buf[3] << 8) + receive_msg_buf[2]);

            if (DEVICE_ID == expected_id)
            {
                switch (receive_command)
                {
                    case 0:
                        // switch output
                        if (0 == receive_msg_buf[4])
                        {
                            port_a_driver_turn_output_high(0);
                        }
                        else if (1 == receive_msg_buf[4])
                        {
                            port_a_driver_turn_output_low(0);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
        
    return 0;
}
