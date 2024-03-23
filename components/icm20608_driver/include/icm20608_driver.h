#ifndef ICM20608_DRIVER_H
#define ICM20608_DRIVER_H
#include "stdint.h"

/* TODO: I still need to completely define possible register values for the
 * configuration registers 
 */


/* TODO: The declarations for the below functions are done, but no implementations
 * have been created yet.
 */

#define ICM20608_DEFAULT_ADDR (0x68)

/* Register definitions*/
#define ACC_XOUT_H_R (0x3B)
#define ACC_YOUT_H_R (0x3D)
#define ACC_ZOUT_H_R (0x3F)
#define GYRO_XOUT_H_R (0x43)
#define GYRO_YOUT_H_R (0x45)
#define GYRO_ZOUT_H_R (0x47)
#define PWR_MGMT_1_R (0x6B)
#define PWR_MGMT_2_R (0x6C)
#define CONFIG_R (0x1A)
#define GYRO_CONFIG_R (0x1B)
#define ACC_CONFIG_1_R (0x1C)
#define ACC_CONFIG_2_R (0x1D)
#define WHO_AM_I_R (0x75)

#define DEFAULT_POWER_MGMT_CONFIG1 (1)
#define DEFAULT_POWER_MGMT_CONFIG2 (0)
#define DEFAULT_GYRO_CONFIG (0x08)
#define DEFAULT_ACC_CONFIG1 (0)
#define DEFAULT_ACC_CONFIG2 (0)
#define DEFAULT_TIMEOUT_TICKS (1000/ portTICK_PERIOD_MS)

#define AD0_HI (1)
#define AD0_LOW (0)
#define DEFAULT_ACC_FACTOR (9.8/16384.0)
#define DEFAULT_GYRO_FACTOR (1.0/131.0)

/* This is a structure for defining some of the characteristics of the IMU */
typedef struct 
{
	uint8_t address;
	uint8_t i2c_port;
	uint8_t gyro_config;
	uint8_t accel_config1;
	uint8_t accel_config2;
	uint8_t power_mgmt_config1;
	uint8_t power_mgmt_config2; 
	float acc_scale_factor;
	float gyro_scale_factor;

} imu_config_t;


void icm20608_init(imu_config_t* imu_config, uint8_t i2c_port);

uint8_t icm20608_whoami(imu_config_t* imu_config);

void icm20608_get_motion_scaled(imu_config_t* imu_config, float gyro_readings[3], float acc_readings[3]);

void icm20608_get_motion_raw(imu_config_t* imu_config, int16_t gyro_readings[3], int16_t acc_readings[3]);

#endif /* icm20608_driver */
