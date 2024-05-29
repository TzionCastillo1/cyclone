#include "mavlink_driver.h"

void mavlink_read_task(void* pvParameters)
{
    while (true)
    {
        char buffer[2048]; //TODO: change this buffer to be set by a macro

        const int ret = recvfrom(this.socket_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

        if (ret < 0)
        {
            ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
            break;
        }
        else if (ret == 0) {
            // If we do not receive anything return
            return;
        }

        *source_addr_set = true; //TODO: create this flag variable

        mavlink_message_t message;
        mavlink_status_t status;

        for (int i = 0; i < ret; ++i)
        {
            if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &message, &status) == 1)
            {
                switch (message.msgid)
                {
                    case MAVLINK_MSG_ID_HEARTBEAT:
                        handle_hearbeat(&message);
                        break;
                    case MAVLINK_MSG_ID_MANUAL_CONTROL:
                        handle_manual_control(&message);
                        break;
                    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
                        handle_param_request_list(&message);
                        break;
                    case MAVLINK_MSG_ID_COMMAND_LONG:
                        handle_command_long(this, &message);
                        break;
                    case MAVLINK_MSG_ID_COMMAND_INT:
                        handle_command_long(this, &message);
                        break;

                }
            }
        }
    }
}


void mavlink_driver_init(mavlink_config_t* this, int socket_fd, const struct sockaddr_in* dest_addr, const uint8_t system_id)
{
    this->socket_fd = socket_fd;
    this->dest_addr = *dest_addr;
    this->dest_addr_len = sizeof(dest_addr);
    this->system_id = system_id;
}

void send_heartbeat(mavlink_config_t* this)
{
    mavlink_message_t message;

    mavlink_msg_heartbeat_pack_chan(
        this->system_id,
        MAV_COMP_ID_AUTOPILOT1,
        MAVLINK_COMM_0,
        &message,
        MAV_TYPE_GENERIC,
        MAV_AUTOPILOT_GENERIC,
        MAV_MODE_FLAG_TEST_ENABLED,
        0,
        MAV_STATE_STANDBY);

    mavlink_send_message_udp(this, message);    
}

int send_message_udp(const mavlink_config_t* this, mavlink_message_t* message)
{
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    const int len = mavlink_msg_to_send_buffer(buffer, &message);

    int ret = sendto(this->socket_fd, buffer, len, 0, (const struct sockaddr*) this->dest_addr, this->dest_addr_len);
    return ret;
}

void send_message_command_ack(const mavlink_config_t* this, const uint16_t command_id, const mavlink_message_t* cmd_message, uint8_t res)
{
    mavlink_message_t message;

    mavlink_msg_command_ack_pack_chan(
        this->system_id,
        MAV_COMP_ID_AUTOPIOT1,
        MAVLINK_COMM_0,
        &message,
        command_id,
        res,
        0,
        time(NULL),
        cmd_message->sysid,
        cmd_message->compid);
    
    ret = mavlink_send_message_udp(this, message);
}

void handle_manual_control(const mavlink_message_t* message_manual_control)
{
    //TODO: Put this message onto the pub-sub queue.
}

void handle_heartbeat(const mavlink_message_t* message_heartbeat)
{
    //TODO: Put this message onto the pub-sub queue.
}

void handle_param_request_list(const mavlink_message_t* message_param_request_list)
{

}

void handle_command_long(mavlink_config_t* this, const mavlink_message_t* message_command_long)
{
    mavlink_command_long_t command_long;
    mavlink_msg_command_long_decode(message_command_long);

    uint8_t res = MAV_RESULT_UNSUPPORTED;
    switch(command_long.command)
    {
        case MAV_CMD_COMPONENT_ARM_DISARM:
            res = handle_component_arm_disarm(this, &command_long);
            break;
        case MAV_CMD_CONTROL_HIGH_LATENCY:
            res = handle_control_high_latency(this, &command_long);
            break;
        case MAV_CMD_DO_FLIGHTTERMINATION:
            res = handle_do_flighttermination:
            break;
    }

    send_message_command_ack(this, (uint16_t) command_long.command, message_command_long, res);
}

void handle_command_int(mavlink_config_t* this, const mavlink_message_t* message_command_int)
{
    mavlink_command_long_t command_long;
    mavlink_msg_command_long_decode(message_command_long);

    uint8_t res = MAV_RESULT_UNSUPPORTED;
    switch(command_long.command)
    {
        case MAV_CMD_COMPONENT_ARM_DISARM:
            res = handle_component_arm_disarm(this, &command_long);
            break;
        case MAV_CMD_CONTROL_HIGH_LATENCY:
            res = handle_control_high_latency(this, &command_long);
            break;
        case MAV_CMD_DO_FLIGHTTERMINATION:
            res = handle_do_flighttermination(this, &command_long);
            break;
    }

    send_message_command_ack(this, (uint16_t) command_long.command, message_command_int, res);
}

uint8_t handle_do_flighttermination(mavlink_t* this, const mavlink_command_long_t* command_do_flighttermination)
{
    //TODO: Put this message onto the pub-sub queue.
}

uint8_t handle_component_arm_disarm(mavlink_config_t* this, const mavlink_command_long_t* command_arm_disarm)
{
    //TODO: Put this message onto the pub-sub queue.
}

uint8_t handle_control_latency(mavlink_config_t* this, const mavlink_command_long_t* command_control_high_latency)
{
    //TODO: Nothing for now.
}