#include "stdint.h"

//#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#include "mat_utils.h"
#include "ekf_imu.h"
//#include "i2c_helper.h"
//#include "icm20608_driver.h"
#include "bdc_motor.h"

const char* TAG = "Cyclone";

#define BDC_MCPWM_TIMER_RESOLUTION_HZ 10000000 // 10MHz
#define BDC_MCPWM_FREQ_HZ 25000 // 25kHz
#define BDC_MCPWM_DUTY_TICK_MAX (BDC_MCPWM_TIMER_RESOLUTION_HZ / BDC_MCPWM_FREQ_HZ)
#define BDC_MCPWM_MOTOR_A 4
#define BDC_MCPWM_MOTOR_B 5
#define BDC_MCPWM_MOTOR_C 6
#define BDC_MCPWM_MOTOR_D 7

#define BDC_PID_LOOP_PERIOD_MS 10

void app_main(void)
{
	/* MOTOR A */
	ESP_LOGI(TAG, "Create DC Motor A ");
	bdc_motor_config_t motor_A_config = {
		.pwm_freq_hz = BDC_MCPWM_FREQ_HZ,
		.pwma_gpio_num = BDC_MCPWM_MOTOR_A,
	};
	bdc_motor_mcpwm_config_t mcpwm_config = {
		.group_id = 0,
		.resolution_hz = BDC_MCPWM_TIMER_RESOLUTION_HZ,
	};

	bdc_motor_handle_t motor_A = NULL;
	ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_A_config, &mcpwm_config, &motor_A));

	ESP_ERROR_CHECK(bdc_motor_enable(motor_A));

	ESP_LOGI(TAG, "Start motor A");
	bdc_motor_set_speed(motor_A, 100);
	vTaskDelay(pdMS_TO_TICKS(1000));
	ESP_LOGI(TAG, "Stop motor A");
	bdc_motor_set_speed(motor_A, 0);

	/* MOTOR B */
	ESP_LOGI(TAG, "Create DC Motor B");
	bdc_motor_config_t motor_B_config = {
		.pwm_freq_hz = BDC_MCPWM_FREQ_HZ,
		.pwma_gpio_num = BDC_MCPWM_MOTOR_B,
	};

	bdc_motor_handle_t motor_B = NULL;
	ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_B_config, &mcpwm_config, &motor_B));

	ESP_ERROR_CHECK(bdc_motor_enable(motor_B));

	ESP_LOGI(TAG, "Start motor B");
	bdc_motor_set_speed(motor_B, 100);
	vTaskDelay(pdMS_TO_TICKS(1000));
	ESP_LOGI(TAG, "Stop motor B");
	bdc_motor_set_speed(motor_B, 0);

	/* MOTOR C */
	ESP_LOGI(TAG, "Create DC Motor C");
	bdc_motor_config_t motor_C_config = {
		.pwm_freq_hz = BDC_MCPWM_FREQ_HZ,
		.pwma_gpio_num = BDC_MCPWM_MOTOR_C,
	};

	bdc_motor_handle_t motor_C = NULL;
	ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_C_config, &mcpwm_config, &motor_C));

	ESP_ERROR_CHECK(bdc_motor_enable(motor_C));

	ESP_LOGI(TAG, "Start motor C");
	bdc_motor_set_speed(motor_C, 100);
	vTaskDelay(pdMS_TO_TICKS(1000));
	ESP_LOGI(TAG, "Stop motor C");
	bdc_motor_set_speed(motor_C, 0);

	/* MOTOR D */
	bdc_motor_mcpwm_config_t mcpwm_D_config = {
		.group_id = 1,
		.resolution_hz = BDC_MCPWM_TIMER_RESOLUTION_HZ,
	};

	ESP_LOGI(TAG, "Create DC Motor D");
	bdc_motor_config_t motor_D_config = {
		.pwm_freq_hz = BDC_MCPWM_FREQ_HZ,
		.pwma_gpio_num = BDC_MCPWM_MOTOR_D,
	};

	bdc_motor_handle_t motor_D = NULL;
	ESP_ERROR_CHECK(bdc_motor_new_mcpwm_device(&motor_D_config, &mcpwm_D_config, &motor_D));

	ESP_ERROR_CHECK(bdc_motor_enable(motor_D));

	ESP_LOGI(TAG, "Start motor D");
	bdc_motor_set_speed(motor_D, 100);
	vTaskDelay(pdMS_TO_TICKS(1000));
	ESP_LOGI(TAG, "Stop motor D");
	bdc_motor_set_speed(motor_D, 0);

	ESP_LOGI(TAG, "All motor test");
	bdc_motor_set_speed(motor_A, 200);
	bdc_motor_set_speed(motor_B, 200);
	bdc_motor_set_speed(motor_C, 200);
	bdc_motor_set_speed(motor_D, 200);
	vTaskDelay(pdMS_TO_TICKS(1000));
	bdc_motor_set_speed(motor_A, 0);
	bdc_motor_set_speed(motor_B, 0);
	bdc_motor_set_speed(motor_C, 0);
	bdc_motor_set_speed(motor_D, 0);
}


