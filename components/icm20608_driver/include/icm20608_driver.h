#ifndef ICM20608_DRIVER_H
#define ICM20608_DRIVER_H


/* TODO: I still need to completely define possible register values for the
 * configuration registers 
 */


/* TODO: The declarations for the below functions are done, but no implementations
 * have been created yet.
 */


#define AD0_HI = 1
#define AD0_LOW = 0
#define DEFAULT_GYRO_CONFIG = 0b00001000
#define DEFAULT_ACCEL_CONFIG1 = 0
#define DEFAULT_ACCEL_CONFIG2 = 0

/* This is a structure for defining some of the characteristics of the IMU */
typedef struct 
{
	uint8_t ad0_level;
	uint8_t gyro_config = DEFAULT_GYRO_CONFIG;
	uint8_t accel_config1 = DEFAULT_ACCEL_CONFIG1;
	uint8_t accel_config2 = DEFAULT_ACCEL_CONFIG2;
	uint8_t power_mgmt_config1 = DEFAULT_POWER_MGMT_CONFIG1;
	uint8_t power_mgmt_config2 = DEFAULT_POWER_MGMT_CONFIG2; 
} imu_config_t;


void icm20608_init(imu_config_t* imu_config);

void icm20608_whoami(imu_config_t* imu_config);

void icm20608_get_motion(imu_config_t* imu_config);

#endif /* icm20608_driver */
