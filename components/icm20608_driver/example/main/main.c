#include "icm20608_driver.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

const char* TAG = "ICM20608 Example";

void init_i2c()
{
	ESP_LOGI(TAG, "Initializing i2c");
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 10,
		.scl_io_num = 11,
		.sda_pullup_en = GPIO_PULLUP_DISABLE,
		.scl_pullup_en = GPIO_PULLUP_DISABLE,
		.clk_flags=0
	};
	conf.master.clk_speed = 400000;
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
}

void app_main(void)
{
	ESP_LOGI(TAG, "starting app_main");

	init_i2c();
	imu_config_t imu_config;
	icm20608_init(&imu_config, I2C_NUM_0);

	uint8_t icm20608_address = icm20608_whoami(&imu_config);
	ESP_LOGI(TAG, "Who am I: %d", icm20608_address);

	float gyro_readings_scaled[3];
	float acc_readings_scaled[3];

	int16_t gyro_readings_raw[3];
	int16_t acc_readings_raw[3];

	while(true)
	{
		icm20608_get_motion_raw(&imu_config, gyro_readings_raw, acc_readings_raw);
		ESP_LOGI(TAG, "gyro readings raw:");
		for (int axis = 0; axis < 3; axis++)
			ESP_LOGI(TAG, "%d", gyro_readings_raw[axis]);

		ESP_LOGI(TAG, "acc readings raw:");
		for (int axis = 0; axis < 3; axis++)
			ESP_LOGI(TAG, "%d", acc_readings_raw[axis]);

		icm20608_get_motion_scaled(&imu_config, gyro_readings_scaled, acc_readings_scaled);
		ESP_LOGI(TAG, "gyro readings scaled:");
		for (int axis = 0; axis < 3; axis++)
			ESP_LOGI(TAG, "%f", gyro_readings_scaled[axis]);

		ESP_LOGI(TAG, "acc readings scaled:");
		for (int axis = 0; axis < 3; axis++)
			ESP_LOGI(TAG, "%f", acc_readings_scaled[axis]);

		vTaskDelay(500/portTICK_PERIOD_MS);
	}

}
