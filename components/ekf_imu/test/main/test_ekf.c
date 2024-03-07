#include "ekf_imu.h"
#include <stdint.h>
#include "unity.h"
#include <stdio.h>
#include "array_utils.h"

void test_get_F()
{

	float state[7] = {1,0,0,0,0,0,0};
	float xTest[7];
	ekf_imu_filter_t ekf_imu_filter={.x={7,1,xTest}};
	//ekf_imu_filter.x.mat = xTest;
	//ekf_imu_filter.x.HEIGHT = 7;
	//ekf_imu_filter.x.WIDTH = 1;
	array_utils_memcpy(state, ekf_imu_filter.x.mat, 7);
	float Fmat[49];
	
	matrixf_t F = {.mat= Fmat, .HEIGHT = 7, .WIDTH = 7};

	float gyro_rates[3] = {0, 0, -1.572604f};
	ekf_imu_get_F(&ekf_imu_filter, F, gyro_rates, 1/20);
}
