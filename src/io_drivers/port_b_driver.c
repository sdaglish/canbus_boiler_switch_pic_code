#include "port_b_driver.h"
#include "xc.h"

static const uint8_t PIN_INDEX_MAX = 15;

static inline uint16_t clear_bit(uint16_t value, uint8_t bit)
{
    value &= ~(1 << bit);
    return value;
}

static inline uint16_t set_bit(uint16_t value, uint8_t bit)
{
    value |= (1 << bit);
    return value;
}

static inline uint16_t and_together(uint16_t x, uint16_t y)
{
    return (x & y);
}

static inline uint16_t invert(uint16_t v)
{
    return (v ^ 0xFFFF);
}

static inline bool check_bit(uint16_t value, uint8_t bit)
{
    return (value & (1 << bit));
}

static inline bool pin_is_set_to_output(uint8_t pin)
{
    if (false == check_bit(TRISB, pin))
    {
        return true;
    }
    return false;
}

static inline bool pin_within_bounds(uint8_t pin)
{
    if (pin <= PIN_INDEX_MAX)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void port_b_driver_set_all_to_output(void)
{
    TRISB = 0x0000;
    LATB  = 0x0000;
}

void port_b_driver_set_all_to_input(void)
{
    TRISB = 0xFFFF;
}

uint8_t port_b_driver_set_pin_to_output(uint8_t pin)
{
    uint8_t success = ok;
    if (pin < 16)
    {
        TRISB = clear_bit(TRISB, pin);
        LATB  = clear_bit(LATB, pin);
    }
    else
    {
        success = too_high;
    }

    return success;
}

uint8_t port_b_driver_set_pin_to_input(uint8_t pin)
{
    uint8_t success = ok;
    if (pin < 16)
    {
        TRISB = set_bit(TRISB, pin);
    }
    else
    {
        success = too_high;
    }

    return success;
}

void port_b_driver_turn_all_outputs_high(void)
{
    LATB = and_together(0xFFFF, invert(TRISB));
}

void port_b_driver_turn_all_outputs_low(void)
{
    LATB = and_together(LATB, TRISB);
}

uint8_t port_b_driver_turn_output_high(uint8_t pin)
{
    uint8_t success = ok;
    if (true == pin_within_bounds(pin))
    {
        if (true == pin_is_set_to_output(pin))
        {
            LATB    = set_bit(LATB, pin);
            success = ok;
        }
    }
    else
    {
        success = too_high;
    }
    return success;
}

uint8_t port_b_driver_turn_output_low(uint8_t pin)
{
    uint8_t success = ok;
    if (true == pin_within_bounds(pin))
    {
        if (true == pin_is_set_to_output(pin))
        {
            LATB    = clear_bit(LATB, pin);
            success = ok;
        }
    }
    else
    {
        success = too_high;
    }
    return success;
}