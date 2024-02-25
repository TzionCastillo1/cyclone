#include "ekf_imu.h"

/* NOTE: The below matrices are square and are therefore only defined by one "size" value */

#define P_SIZE 7
#define F_SIZE 7
#define H_HEIGHT 3
#define H_WIDTH 7

typedef struct
{
	float R[9];
	float Q[49];
	float K[21];
	float P_prediction[49];
	float P_estimate[49];
	float x_prediction[7];
	float x_estimate[7];

} ekf_imu_filter_t;

ekf_imu_filter_t* ekf_imu_init(const ekf_imu_config_t* ekf_imu_config)
{
	ekf_imu_filter_t* ekf_imu_filter = {
		.R = ekf_imu_config->R_init,
		.Q = ekf_imu_config->Q_init,
		.P_estimate = ekf_imu_config->P_init,
		.x_estimate = ekf_imu_config->x_init
	};

	return ekf_imu_filter;
}

/* NOTE: The gyro readings are in units of rad/s */
void ekf_imu_predict(const float* gyro_readings, ekf_imu_filter_t* ekf_imu_filter, const float delta_t)
{
	float P_temp[P_SIZE*P_SIZE];

	/* TODO: Provide the state function jacobian, F_x, for this function to use. Options include
	 * passing a function pointer to this funciton which would calculate F_x given x, or building this functionality
	 * in as a part of this function*/

	dspm_mult_f32_aes3(F, P_estimate, P_temp, F_SIZE, F_SIZE, P_SIZE);
	float F_transpose[F_SIZE*F_SIZE];
	mat_utils_transpose(F, F_transpose, F_SIZE, F_SIZE);
	dspm_mult_f32_aes3(P_temp, F_inv, ekf_imu_filter->P_prediction, P_SIZE, P_SIZE, F_SIZE);
	mat_utils_skew_quat(x_estimate, skew_q_curr);
	
	/* NOTE: We are using the kinematic quaternion equation to update the quaternion through angular velocities.
	 * This assumes that the gyro readings and gyro biases are provided to the filter in rad/s. It works through 
	 * small angle approximations of the trigonometric functions that can be used to describe a rotation quaternion */

	float q_gyro_rates[4];
	dspm_mult_f32_aes3(skew_q_curr, gyro_readings, q_gyro_rates, 4,3,1);
	mat_utils_c_mult_scalar(q_gyro_rates, (dt/2.0f), 4, 1);

	/* NOTE: The second parameter of the following function call shifts the pointer ahead by 
	 * 4 spaces in order to access the gyro bias portion of the state*/
	float q_gyro_bias[4];
	dspm_mult_f32_aes3(skew_q_curr, ekf_imu_filter->x_estimate + 4, q_gyro_bias, 4, 3, 1);
	mat_utils_c_mult_scalar(q_gyro_rates, (-dt/2.0f), 4, 1);
	
	x_prediction = x_estimate;

	mat_utils_c_add(x_estimate, q_gyro_rates, 4, 1);
	mat_utils_c_add(x_estimate, q_gyro_bias, 4, 1);

}

void ekf_imu_update(const float* accel_readings, ekf_imu_filter_t* ekf_imu_filter)
{
	/* TODO: This function will implement the code to innovate, or filter, our predicted state.
	 * This will be done through the use of Jacobians for the non-linear state and measurement 
	 * equations, f(x)' = F_x, h(x)' = H_x. These jacobians are used to provide a linear estimate
	 * of the actual functions, centered at the current state. This linearity is a requirement for
	 * the use of a kalman filter, as otherwise we would be transforming the gaussian noise random
	 * variables into some non-gaussian pdf. */
	// NOTE: K = P_prediction*H_transpose*(H*P_prediction*H_transpose + R)^-1

	float H_transpose[H_WIDTH*H_HEIGHT];
	mat_utils_transpose(H, H_transpose, H_HEIGHT, H_WIDTH);
	// NOTE: K_temp_1 = P_prediction * H_tranpose
	float K_temp_1[7*3];
	dspm_mult_f32_aes3(ekf_imu_filter->P_prediction, H_transpose, K_temp_1 P_SIZE, P_SIZE, H_HEIGHT);

	// NOTE: K_temp_2 = H*K_temp_1
	float K_temp_2[7*3];
	dspm_mult_f32_aes3(H, K_temp_1, K_temp_2, H_HEIGHT, H_WIDTH, H_HEIGHT);

	// NOTE: K_temp_2 = K_temp_2 + R
	mat_utils_c_add(K_temp_2, R, H_HEIGHT, H_HEIGHT);

	// NOTE: K_temp_2 = (K_temp_2)^-1
	mat_utils_inverse(K_temp_2, K_temp_2, H_HEIGHT);

	dspm_mult_f32_aes3(K_temp_1, K_temp_2, K, K_HEIGHT, K_WIDTH, K_WIDTH);

	ekf_imu_filter->x_estimate = ekf_imu_filter->x_prediction;

	// NOTE: y_innovation = y - h(x)
	float y_innovation[3*1];
	mat_utils_c_sub(accel_readings, h, 3, 1);

	// NOTE: x_temp = K*(y_innovation - h(x)
	float x_temp[7*1];
	dspm_mult_f32_aes3(K, y_innovation, x_temp, K_HEIGHT, K_WIDTH, 1);

	// NOTE: x_estimate = x_prediction + x_temp
	mat_utils_c_add(x_estimate, x_temp, 7, 1);

	mat_utils_q_normalize(x_estimate);

	// NOTE: P_estimate_temp_1 = K*H
	float P_estimate_temp_1[P_SIZE*P_SIZE];
	dspm_mult_f32_aes3(K, H, P_estimate_temp_1, K_HEIGHT, K_WIDTH, H_WIDTH);
	
	// NOTE: P_estimate_temp_2 = I - P_estimate_temp_1
	float P_estimate_temp_2[P_SIZE*P_SIZE];
	mat_utils_eye(P_estimate_temp_2, P_SIZE);
	mat_utils_c_sub(P_estimate_temp_2, P_estimate_temp_1, P_SIZE, P_SIZE);

	// NOTE: P_estimate = P_estimate_temp_2 * P_prediction
	dspm_mult_f32_aes3(P_estimate_temp_2, P_prediction, P_estimate, P_SIZE, P_SIZE);
}

void ekf_imu_get_state(float* x_output, ekf_imu_filter_t* ekf_imu_filter)
{
	*x_output = ekf_imu_filter_params->x_estimate;
}

