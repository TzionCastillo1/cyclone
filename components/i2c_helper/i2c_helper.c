#include "i2c_helper.h"
#include "driver/i2c.h"

esp_err_t i2c_helper_init(const i2c_config_t* p_config, i2c_port_t i2c_port)
{
	i2c_param_config(i2c_port, p_config);
	return i2c_driver_install(i2c_port, I2C_MODE_MASTER, 0, 0, 0);
}

esp_err_t i2c_helper_read_bytes(i2c_port_t i2c_port, uint8_t device_address, uint8_t reg_address, uint8_t read_length, uint8_t* p_read_buf)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	/* Shifting address and making LSB a 1 to indicate a read. */
	i2c_master_write_byte(cmd, device_address << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg_address, 1);
	i2c_master_start(cmd);
	uint16_t count = 0;
	for(; count < read_length; count++)
	{
		i2c_master_write_byte(cmd, device_address << 1 | I2C_MASTER_READ, ACK_CHECK_EN);
		i2c_master_read_byte(cmd, p_read_buf + count, ACK_VAL);
	}
	i2c_master_read_byte(cmd, p_read_buf + count, NACK_VAL);
	i2c_master_stop(cmd);
	esp_err_t err_value = i2c_master_cmd_begin(i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	return err_value;
}

esp_err_t i2c_helper_write_bytes(i2c_port_t i2c_port, uint8_t device_address, uint8_t reg_address, uint8_t write_length, uint8_t p_write_buf)
{
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	/* shifting address to the left by one and making the LSB a 0 to indicate a write. */
	i2c_master_write_byte(cmd, device_address << 1 | I2C_MASTER_WRITE, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, reg_address, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, p_write_buf, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	esp_err_t err_value = i2c_master_cmd_begin(i2c_port, cmd, 100 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	return err_value;

}
	
/*
esp_err_t i2c_helper_write_byte(i2c_port_t i2c_port, uint8_t device_address, uint8_t reg_address, uint8_t p_write_buf)
{
	return i2c_helper_write_bytes(i2c_port, device_address, reg_address, 1, p_write_buf);
}

esp_err_t i2c_helper_read_byte(i2c_port_t i2c_port, uint8_t device_address, uint8_t reg_address, uint8_t* p_read_buf);
{
	return i2c_helper_read_bytes(i2c_port, device_address, reg_address, 1, p_read_buf);
}

*/
