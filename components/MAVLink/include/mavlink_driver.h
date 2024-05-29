#include <mavlink/common/mavlink.h>

#include <stdint.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

typedef struct
{
    int socket_fd;
    struct sockaddr_in dest_addr;
    socklen_t dest_addr_len;
}mavlink_config_t;

void mavlink_read_task(void *pvParameters);
void mavlink_driver_init(mavlink_config_t* this, int socket_fd, const struct sockaddr_in* dest_addr, const uint8_t system_id);
// Sticking with just basic UDP messaging for now. I have some ideas on how to abstract
// this to work with uart more easily, but not worth pursuing right now.
void send_message_udp(const mavlink_config_t* this, const mavlink_message_t* message);

void send_message_command_ack(const mavlink_config_t* this, uint16_t command_id, const mavlink_message_t* message, uint8_t res);

// This method may need to be alled periodically by a timer interrupt.
// It could also be setup as it's own task, but I am not sure that it would
// be worth all of the setup and overhead that that would entail.
void send_heartbeat(mavlink_config_t* this);

// Methods to process data from GCS
// MAVLink Commands need to return a MAV_RESULT
// --Messages--
// MANUAL_CONTROL
void handle_manual_control(const mavlink_message_t* message_manual_control);
// HEARTBEAT
void handle_heartbeat(const mavlink_message_t* message_heartbeat);
// PARAM_REQUEST_LIST
void handle_param_request_list(const mavlink_message_t* message_param_request_list);
// COMMAND_LONG
void handle_command_long(const mavlink_message_t* message_command_long);
//COMMAND_INT
void handle_command_int(const mavlink_message_t* message_command_int);

// --Commands--
// MAV_CMD_COMPONENT_ARM_DISARM
uint8_t handle_component_arm_disarm(const mavlink_command_long_t* command_arm_disarm);
//MAV_CMD_CONTROL_HIGH_LATENCY
uint8_t handle_control_high_latency(mavlink_command_long_t* command_control_high_latency);
