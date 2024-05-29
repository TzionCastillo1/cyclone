#include "stdint.h"
#include "stdio.h"
#include "string.h"

//#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

#include "i2c_helper.h"
#include "icm20608_driver.h"

const char* TAG = "Cyclone";
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#define configTICK_RATE_HZ 100

#define DEBUG_STACK_SIZE 2048
#define IMU_STACK_SIZE 2048

/*** Queue Struct ***/
typedef struct{
	void* pData;
	uint8_t usDataID;
} xQueueData_t;

/*** Task Queues ***/
QueueHandle_t xDebug_task_queue;

/*** Setup Functions***/
void init_i2c()
{
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = 10,
		.scl_io_num = 11,
		.sda_pullup_en = GPIO_PULLUP_DISABLE,
		.scl_pullup_en = GPIO_PULLUP_DISABLE,
		.clk_flags = 0
	};

	conf.master.clk_speed = 400000;
	ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &conf));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
}

/*** Task to periodically check for MAVLink messages ***/
void vMAVLink_read_task(void* pvParameters)
{

	for(;;)
	{

	}
	vTaskDelete(NULL);
}

/*** Task to periodically read from IMU ***/
/*** TODO: This task will be converted into the EKF task*/
static imu_config_t imu_config;
void vIMU_Read_Task(void *pvParameters )
{
	imu_config_t* imu_config_1 = (imu_config_t*) pvParameters;

	TickType_t xLastWakeTime;
	const TickType_t xTimeIncrement = pdMS_TO_TICKS(10);

	//Initialize the xLastWakeTime variable with the current time
	xLastWakeTime = xTaskGetTickCount();

	imu_values_t imu_current_values;
	BaseType_t xStatus;

	xQueueData_t imu_data;
	imu_data.usDataID = 0x01; //TODO: define these message ID's somewhere
	for(;;)
	{
		icm20608_get_motion_scaled(imu_config_1, &imu_current_values);
		imu_data.pData = (void*) &imu_current_values;
		xStatus = xQueueSendToBack(xDebug_task_queue, &imu_data, 0);

		xTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
	vTaskDelete(NULL);
}

/*** Task to periodically print debug statements ***/

void vDebug_Queue_read_Task(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xTimeIncrement = pdMS_TO_TICKS(10);

	BaseType_t xStatus;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

	xQueueData_t imu_data;
	imu_values_t imu_values;

	//Initialize the xLastWakeTime variable with the current time
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
		xStatus = xQueueReceive(xDebug_task_queue, &imu_data, xTicksToWait);
		if(xStatus == pdPASS)
		{
			memcpy((void*) &imu_values, (void*) imu_data.pData, sizeof(imu_values_t));
			imu_values = (imu_values_t) imu_values;
			ESP_LOGI(TAG, "Received: %f", imu_values.acc_x);
		}
		else
		{
			//ESP_LOGI(TAG, "Received nothing from the Queue.");
		}
		xTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
	vTaskDelete(NULL);
}

void app_main(void)
{
	xDebug_task_queue = xQueueCreate(5, sizeof(imu_values_t));

	TaskHandle_t xDebug_Handle = NULL;
	xTaskCreate(vDebug_Queue_read_Task, "DEBUG", DEBUG_STACK_SIZE, NULL, 2, &xDebug_Handle);

	init_i2c();
	icm20608_init(&imu_config, I2C_NUM_0);
	TaskHandle_t xIMU_Handle = NULL;
	xTaskCreate(vIMU_Read_Task, "IMU", IMU_STACK_SIZE, &imu_config, 1, &xIMU_Handle);

}