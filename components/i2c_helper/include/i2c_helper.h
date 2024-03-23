#ifndef I2C_HELPER_H
#define I2C_HELPER_H

#include "stdint.h"
#include "driver/i2c.h"

#define ACK_CHECK_EN 0x01
#define ACK_CHECK_DIS 0x00
#define ACK_VAL 0x00
#define NACK_VAL 0x01

esp_err_t i2c_helper_init(const i2c_config_t* config, i2c_port_t i2c_port_t);

esp_err_t i2c_helper_read_bytes(i2c_port_t i2c_port, uint8_t device_address, uint8_t reg_address, uint8_t length, uint8_t* p_read_buf);

esp_err_t i2c_helper_write_bytes(i2c_port_t i2c_port, uint8_t device_address, uint8_t reg_address, uint8_t length, uint8_t p_write_buf);

/*
esp_err_t i2c_helper_write_byte(i2c_port_t i2c_port, uint8_t device_address, uint8_t reg_address, uint8_t p_write_buf);
*/

#endif /* I2C_HELPER_H */
