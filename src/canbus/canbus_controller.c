#include "canbus_controller.h"

static uint16_t device_id = 0xFFFF;
static baudrate_t device_baudrate = br_num_types;
static uint16_t receive_id = 0x0000;
static uint8_t receive_msg_len = 0x00;
static uint8_t receive_msg[8];
//static uint16_t msg_from_id = 0;

bool canbus_controller_init(uint16_t id, baudrate_t br)
{
    if (id > MAX_ID)
    {
        return false;
    }
    device_id = id;

    if (br >= br_num_types)
    {
        return false;
    }
    device_baudrate = br;

    bool success = false;

    do 
    {
        success = mcp2515_init();

    } while (false == success);
    return success;

}

uint16_t canbus_controller_get_device_id(void)
{
    return device_id;
}

baudrate_t canbus_controller_get_baudrate(void)
{
    return device_baudrate;
}

bool canbus_controller_send_switch_on(id_t id)
{
    if (id >= id_num_types)
    {
        return false;
    }
    uint8_t message[] = {0, 0, (id_boiler_switch & 0x00FF), (id_boiler_switch >> 8), 0, 1};
    bool success = mcp2515_driver_send_msg_buffer(device_id, 0, 6, message);
    return success;
}

bool canbus_controller_send_switch_off(id_t id)
{    
    if (id >= id_num_types)
    {
        return false;
    }
    uint8_t message[] = {0, 0, (id_boiler_switch & 0x00FF), (id_boiler_switch >> 8), 0, 0};
    bool success = mcp2515_driver_send_msg_buffer(device_id, 0, 6, message);
    return success;
}


bool canbus_controller_has_receive_data()
{
    bool rx0_has_data = mcp2515_rx0_is_full();
    return rx0_has_data;
}

bool canbus_controller_read_buf(void)
{
    uint8_t tmp_msg[8];
    mcp2515_driver_read_can_message(&receive_id, &receive_msg_len, tmp_msg); 
    uint8_t i = 0;
    for (i = 0; i < receive_msg_len; i++)
    {
        receive_msg[i] = tmp_msg[i];
    }
    mcp2515_driver_clear_rx0if();
    return true;
}

uint16_t canbus_controller_get_buf_from_id(void)
{
    return receive_id;
}

uint8_t canbus_controller_get_receive_msg_len(void)
{
    return receive_msg_len;
}

void canbus_controller_get_receive_msg(uint8_t* p_receive_msg)
{
    uint8_t i = 0;
    for(i = 0; i < receive_msg_len; i++)
    {
        p_receive_msg[i] = receive_msg[i];
    }
}
