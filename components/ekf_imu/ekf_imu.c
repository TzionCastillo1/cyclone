#include "ekf_imu.h"
#include <stdio.h>

#define GRAVITY_CONSTANT (9.8)

void ekf_imu_init(const ekf_imu_config_t* config, ekf_imu_filter_t* this)
{
	this->R.HEIGHT = config->R_init.HEIGHT;
	this->R.WIDTH = config->R_init.WIDTH;
	this->R.mat = this->R_mat;
	mat_utils_copy(&config->R_init, &this->R);

	this->Q.HEIGHT = config->Q_init.HEIGHT;
	this->Q.WIDTH = config->Q_init.WIDTH;
	this->Q.mat = this->Q_mat;
	mat_utils_copy(&config->Q_init, &this->Q);

	this->P.HEIGHT = config->P_init.HEIGHT;
	this->P.WIDTH = config->P_init.WIDTH;
	this->P.mat = this->P_mat;
	mat_utils_copy(&config->P_init, &this->P);

	this->x.HEIGHT = config->x_init.HEIGHT;
	this->x.WIDTH = config->x_init.WIDTH;
	this->x.mat = this->x_mat;
	mat_utils_copy(&config->x_init, &this->x);

}


void ekf_imu_set_default_params(ekf_imu_config_t* config, const float R_INITIAL_DIAG, const float Q_INITIAL_DIAG, const float P_INITIAL_DIAG)
{

	config->R_init.HEIGHT = 3;
	config->R_init.WIDTH = 3;
	config->R_init.mat = config->R;

	config->Q_init.HEIGHT = 7;
	config->Q_init.WIDTH = 7;
	config->Q_init.mat = config->Q;

	config->P_init.HEIGHT = 7;
	config->P_init.WIDTH = 7;
	config->P_init.mat = config->P;

	config->x_init.HEIGHT = 7;
	config->x_init.WIDTH = 1;
	config->x_init.mat = config->x;

	mat_utils_eye(&config->R_init);
	mat_utils_c_mult_scalar(&config->R_init, R_INITIAL_DIAG);

	mat_utils_eye(&config->Q_init);
	mat_utils_c_mult_scalar(&config->Q_init, Q_INITIAL_DIAG);

	mat_utils_eye(&config->P_init);
	mat_utils_c_mult_scalar(&config->P_init, P_INITIAL_DIAG);
}

void ekf_imu_reset(const ekf_imu_config_t* config, ekf_imu_filter_t* this)
{
	ekf_imu_init(config, this);
	mat_utils_zeros(&this->K);
}

void ekf_imu_get_F(ekf_imu_filter_t* this,  matrixf_t* F, float gyro_rates[3], const float dt)
{
	float half_dt = dt/2;
	float adj_rates[3] = {gyro_rates[0] - this->x.mat[4], 
								gyro_rates[1] - this->x.mat[5], 
								gyro_rates[2] - this->x.mat[6]}; 

	float F_tmp_raw[49] = {1, -half_dt*adj_rates[0], -half_dt*adj_rates[1], -half_dt*adj_rates[2], -(this->x.mat[1]), -(this->x.mat[2]), -(this->x.mat[3]),
							half_dt*adj_rates[0], 1, half_dt*adj_rates[2], -half_dt*adj_rates[1], this->x.mat[0], -(this->x.mat[3]), this->x.mat[2],
							half_dt*adj_rates[1], -half_dt*adj_rates[2], 1, half_dt*adj_rates[0], this->x.mat[3], this->x.mat[0], -(this->x.mat[1]),
							half_dt*adj_rates[2], half_dt*adj_rates[1], -half_dt*adj_rates[0], 1, -(this->x.mat[2]), this->x.mat[1], this->x.mat[0],
							0, 0, 0, 0, 1, 0, 0,
							0, 0, 0, 0, 0, 1, 0, 
							0, 0, 0, 0, 0, 0, 1};
	array_utils_memcpy(F_tmp_raw, F->mat, 49); 	
}

void ekf_imu_get_H(ekf_imu_filter_t* this, matrixf_t* H)
{
	float H_tmp_raw[21] = {this->x.mat[2], this->x.mat[3], this->x.mat[0], this->x.mat[1], 0, 0, 0,
							-(this->x.mat[1]), -(this->x.mat[0]), this->x.mat[3], this->x.mat[2], 0, 0, 0,
							this->x.mat[0], -(this->x.mat[1]), -(this->x.mat[2]), this->x.mat[3], 0, 0, 0};
	array_utils_memcpy(H_tmp_raw, H->mat, 21);
	mat_utils_c_mult_scalar(H, 2.0);
}

void ekf_imu_get_h(ekf_imu_filter_t* this, matrixf_t* h)
{
	float h_tmp_raw[3] = {2*((this->x.mat[0] * this->x.mat[2]) + (this->x.mat[1] * this->x.mat[3])),
							2*((this->x.mat[2] * this->x.mat[3]) - (this->x.mat[0] * this->x.mat[1])),
							(this->x.mat[0] * this->x.mat[0]) - (this->x.mat[1] * this->x.mat[1]) - (this->x.mat[2] * this->x.mat[2]) + (this->x.mat[3] * this->x.mat[3])};

	array_utils_memcpy(h_tmp_raw, h->mat, 21);
	mat_utils_c_mult_scalar(h, GRAVITY_CONSTANT);
}

void ekf_imu_predict_state(ekf_imu_filter_t* this, float* gyro_rates, float dt)
{

	float S_q_mat[12];
	matrixf_t S_q = {.mat = S_q_mat, .HEIGHT=4, .WIDTH=3};
	mat_utils_skew_quat(this->x.mat, &S_q, dt);

	float Fmat[49];
	
	matrixf_t F = {.mat= Fmat, .HEIGHT = 7, .WIDTH = 7};

	ekf_imu_get_F(this, &F, gyro_rates, dt);

	float P_tmp_mat[49];
	matrixf_t P_tmp = {.mat = P_tmp_mat, .HEIGHT = 7, .WIDTH = 7};
	mat_utils_mat_mult(&F, &this->P, &P_tmp); 

	float F_transpose_mat[49];
	matrixf_t F_transpose = {.mat = F_transpose_mat, .HEIGHT = 7, .WIDTH = 7};
	mat_utils_transpose(&F, &F_transpose);

	mat_utils_mat_mult(&P_tmp, &F_transpose, &this->P);

	mat_utils_c_add(&this->P, &this->Q);

	//TODO: Consider moving this bias correction process to a seperate function.
	float adj_omega_mat[3] = {gyro_rates[0] - this->x.mat[4], gyro_rates[1] - this->x.mat[5], gyro_rates[2] - this->x.mat[6]}; 
	matrixf_t adj_omega = {.mat = adj_omega_mat, .HEIGHT = 3, .WIDTH = 1};

	float x_quat_add_mat[4];
	matrixf_t x_quat_add = {.mat = x_quat_add_mat, .HEIGHT = 4, .WIDTH = 1};

	mat_utils_mat_mult(&S_q, &adj_omega, &x_quat_add);

	float x_tmp_mat[4];
	matrixf_t x_tmp = {.mat=x_tmp_mat, .HEIGHT = 4, .WIDTH = 1};
	
	mat_utils_copy_block(&x_tmp, 0, 0, &this->x, 0, 0, 3, 0);
	
	mat_utils_c_add(&x_tmp, &x_quat_add);

	mat_utils_copy_block(&this->x, 0, 0, &x_tmp, 0, 0, 3, 0);

}

void ekf_imu_get_K(ekf_imu_filter_t* this, matrixf_t* H, matrixf_t* K)
{
	float H_transpose_mat[21];
	matrixf_t H_transpose = {.mat=H_transpose_mat, .HEIGHT = 7, .WIDTH = 3};
	mat_utils_transpose(H, &H_transpose);
	
	float K_temp_1_mat[21];
	matrixf_t K_temp_1 = {.mat = K_temp_1_mat, .HEIGHT = 7, .WIDTH = 3};
	mat_utils_mat_mult(&this->P, &H_transpose, &K_temp_1);

	float K_temp_2_mat[9];
	matrixf_t K_temp_2 = {.mat=K_temp_2_mat, .HEIGHT = 3, .WIDTH = 3};
	mat_utils_mat_mult(H, &K_temp_1, &K_temp_2);
	
	mat_utils_c_add(&K_temp_2, &this->R);

	float K_temp_3_mat[9];
	matrixf_t K_temp_3 = {.mat=K_temp_3_mat, .HEIGHT=3, .WIDTH=3};
	mat_utils_inverse_3x3(&K_temp_2, &K_temp_3);

	mat_utils_mat_mult(&K_temp_1, &K_temp_3, K);
}

void ekf_imu_update_state(ekf_imu_filter_t* this, float* acc_measurements)
{
	float H_mat[21];
	matrixf_t H = {.mat = H_mat, .HEIGHT = 3, .WIDTH = 7};
	ekf_imu_get_H(this, &H);

	float K_mat[21];
	matrixf_t K = {.mat = K_mat, .HEIGHT = 7, .WIDTH = 3};
	ekf_imu_get_K(this, &H, &K);

	float h_mat[3];
	matrixf_t h = {.mat=h_mat, .HEIGHT = 3, .WIDTH = 1};
	ekf_imu_get_h(this, &h);

	float meas_error_mat[3];
	matrixf_t meas_error = {.mat=meas_error_mat, .HEIGHT = 3, .WIDTH = 1};
	array_utils_memcpy(acc_measurements, meas_error.mat, 3);
	mat_utils_c_sub(&meas_error, &h);

	float x_temp_mat[7];
	matrixf_t x_temp = {.mat=x_temp_mat, .HEIGHT = 7,.WIDTH = 1};
	mat_utils_mat_mult(&K, &meas_error, &x_temp);

	mat_utils_c_add(&this->x, &x_temp);

	mat_utils_quat_norm(this->x.mat);
	// TODO: Update Estimate Error COvariance matrix
	
	float eye_7x7_mat[49];
	matrixf_t eye_7x7 = {.mat=eye_7x7_mat, .HEIGHT = 7, .WIDTH = 7};
	mat_utils_eye(&eye_7x7);

	float P_tmp_1_mat[49];
	matrixf_t P_tmp_1 = {.mat=P_tmp_1_mat, .HEIGHT=7, .WIDTH=7};
	mat_utils_mat_mult(&K, &H, &P_tmp_1);

	mat_utils_c_sub(&eye_7x7, &P_tmp_1);

	float P_tmp_2_mat[49];
	matrixf_t P_tmp_2 = {.mat=P_tmp_2_mat, .HEIGHT=7, .WIDTH=7};
	mat_utils_mat_mult(&eye_7x7, &this->P, &P_tmp_2);

	mat_utils_copy(&P_tmp_2, &this->P);
}
/* NOTE: The gyro readings are in units of rad/s */
