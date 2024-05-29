#include "icm20608_driver.h"
#include "i2c_helper.h"


void icm20608_init(imu_config_t* imu_config, uint8_t i2c_port)
{
	imu_config->address = ICM20608_DEFAULT_ADDR;
	imu_config->i2c_port = i2c_port;
	imu_config->gyro_config = DEFAULT_GYRO_CONFIG;
	imu_config->accel_config1 = DEFAULT_ACC_CONFIG1;
	imu_config->accel_config2 = DEFAULT_ACC_CONFIG2;
	imu_config->power_mgmt_config1 = DEFAULT_POWER_MGMT_CONFIG1;
	imu_config->power_mgmt_config2 = DEFAULT_POWER_MGMT_CONFIG2;
	imu_config->acc_scale_factor = DEFAULT_ACC_FACTOR;
	imu_config->gyro_scale_factor = DEFAULT_GYRO_FACTOR;

	uint8_t msg = imu_config->power_mgmt_config1;
	i2c_helper_write_bytes(imu_config->i2c_port, imu_config->address, PWR_MGMT_1_R, 1, msg);
	msg = imu_config->power_mgmt_config2;
	i2c_helper_write_bytes(imu_config->i2c_port, imu_config->address, PWR_MGMT_2_R, 1, msg);
	msg = 1;
	i2c_helper_write_bytes(imu_config->i2c_port, imu_config->address, CONFIG_R, 1, msg);
	msg = imu_config->gyro_config;
	i2c_helper_write_bytes(imu_config->i2c_port, imu_config->address, GYRO_CONFIG_R, 1, msg);
	msg = 0;
	i2c_helper_write_bytes(imu_config->i2c_port, imu_config->address, ACC_CONFIG_1_R, 1, msg);
	i2c_helper_write_bytes(imu_config->i2c_port, imu_config->address, ACC_CONFIG_2_R, 1, msg);

}

uint8_t icm20608_whoami(imu_config_t* imu_config)
{
	uint8_t tx_data = WHO_AM_I_R;
	uint8_t rx_data[1];
	i2c_master_write_read_device(imu_config->i2c_port, imu_config->address, &tx_data, 1, rx_data, 1, DEFAULT_TIMEOUT_TICKS);
	return *rx_data;
}

void icm20608_get_motion_raw(imu_config_t* config, int16_t gyro_readings[3], int16_t acc_readings[3])
{
	uint8_t buffer[12];
	i2c_helper_read_bytes(config->i2c_port, config->address, ACC_XOUT_H_R, 2, buffer);
	i2c_helper_read_bytes(config->i2c_port, config->address, ACC_YOUT_H_R, 2, buffer + 2);
	i2c_helper_read_bytes(config->i2c_port, config->address, ACC_ZOUT_H_R, 2, buffer + 4);
	i2c_helper_read_bytes(config->i2c_port, config->address, GYRO_XOUT_H_R, 2, buffer + 6);
	i2c_helper_read_bytes(config->i2c_port, config->address, GYRO_YOUT_H_R, 2, buffer + 8);
	i2c_helper_read_bytes(config->i2c_port, config->address, GYRO_ZOUT_H_R, 2, buffer + 10);

	acc_readings[0] = ((int16_t) buffer[0] << 8) | (int16_t) buffer[1];
	acc_readings[1] = ((int16_t) buffer[2] << 8) | (int16_t) buffer[3];
	acc_readings[2] = ((int16_t) buffer[4] << 8) | (int16_t) buffer[5];

	gyro_readings[0] = ((int16_t) buffer[6] << 8) | (int16_t) buffer[7];
	gyro_readings[1] = ((int16_t) buffer[8] << 8) | (int16_t) buffer[9];
	gyro_readings[2] = ((int16_t) buffer[10] << 8) | (int16_t) buffer[11];

}


void icm20608_get_motion_scaled(imu_config_t* config, imu_values_t* imu_values)
{
	int16_t gyro_readings_raw[3];
	int16_t acc_readings_raw[3];
	icm20608_get_motion_raw(config, gyro_readings_raw, acc_readings_raw);

	/* Process and store gyroscope readings*/
	imu_values->gyro_x = gyro_readings_raw[0] * config->gyro_scale_factor;
	imu_values->gyro_y = gyro_readings_raw[1] * config->gyro_scale_factor;
	imu_values->gyro_z = gyro_readings_raw[2] * config->gyro_scale_factor;

	/* Process and store accelerometer readings*/
	imu_values->acc_x = acc_readings_raw[0] * config->acc_scale_factor;
	imu_values->acc_y = acc_readings_raw[1] * config->acc_scale_factor;
	imu_values->acc_z = acc_readings_raw[2] * config->acc_scale_factor;
}
