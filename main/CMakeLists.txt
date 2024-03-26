idf_component_register(SRCS "main.c"
						INCLUDE_DIRS "."
						REQUIRES driver
								bdc_motor
								esp_event
								icm20608_driver
								ekf_imu
								i2c_helper)


