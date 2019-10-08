#include "port_a_driver.h"
#include <xc.h>

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
    if (false == check_bit(TRISA, pin))
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

void port_a_driver_set_all_to_output(void)
{
    TRISA = 0x0000;
    LATA  = 0x0000;
}

void port_a_driver_set_all_to_input(void)
{
    TRISA = 0xFFFF;
}

uint8_t port_a_driver_set_pin_to_output(uint8_t pin)
{
    uint8_t success = ok;
    if (pin < 16)
    {
        TRISA = clear_bit(TRISA, pin);
        LATA  = clear_bit(LATA, pin);
    }
    else
    {
        success = too_high;
    }

    return success;
}

uint8_t port_a_driver_set_pin_to_input(uint8_t pin)
{
    uint8_t success = ok;
    if (pin < 16)
    {
        TRISA = set_bit(TRISA, pin);
    }
    else
    {
        success = too_high;
    }

    return success;
}

void port_a_driver_turn_all_outputs_high(void)
{
    LATA = and_together(0xFFFF, invert(TRISA));
}

void port_a_driver_turn_all_outputs_low(void)
{
    LATA = and_together(LATA, TRISA);
}

uint8_t port_a_driver_turn_output_high(uint8_t pin)
{
    uint8_t success = ok;
    if (true == pin_within_bounds(pin))
    {
        if (true == pin_is_set_to_output(pin))
        {
            LATA    = set_bit(LATA, pin);
            success = ok;
        }
    }
    else
    {
        success = too_high;
    }
    return success;
}

uint8_t port_a_driver_turn_output_low(uint8_t pin)
{
    uint8_t success = ok;
    if (true == pin_within_bounds(pin))
    {
        if (true == pin_is_set_to_output(pin))
        {
            LATA    = clear_bit(LATA, pin);
            success = ok;
        }
    }
    else
    {
        success = too_high;
    }
    return success;
}

uint8_t port_a_driver_pin_is_low(uint8_t pin)
{
    bool is_low = !check_bit(PORTA, pin);
    return is_low;
}

