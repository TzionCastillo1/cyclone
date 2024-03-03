#ifndef EKF_IMU
#define EKF_IMU

#include "eigen3/Eigen/Dense"
#include <cmath>

class ekfImu
{
	public:

		enum ekf_health_t
		{
			/* TODO: COme o=up with common, comprehensive errors */	

		}
		typedef Eigen::Matrix3f Matrix3f;
		typedef Eigen::Matrix<float, 7, 7> Matrix7f;
		typedef Eigen::matrix<float, 4, 3> Matrix4x3f;
		typedef Eigen::Matrix<float, 3, 7> Matrix3x7f;
		typedef Eigen::Matrix<float, 7, 3> Matrix7x3f;
		typedef Eigen::Matrix<float, 7, 1> Vector7f;
		typedef Eigen::Vector4f Vector4f;
		typedef Eigen::Vector3f Vector3f;

		/**
		 * @brief EKF_imu constructor
		 *
		 * @params state_init: Initial state vector
		 * @params P_init: Initial estimate error covariance matrix
		 * @params Q: Initial process noise covariance matrix
		 * @params R: Initial measurement noise covariance matrix
		 *
		 * @return none
		 */
		EKF_imu(Vector7f state_init,
				Matrix7f P_init,
				Matrix7f Q,
				Matrix3f R);
		/**
		 * @brief EKF_imu constructor
		 *
		 * @return none
		 */
		EKF_imu();
		~EKF_imu();

		/**
		 * @brief EKF_imu initializer
		 *
		 * Sets values of EKF to a desired initial state.
		 *
		 * @params state_init: Initial state vector
		 * @params P_init: Initial estimate error covariance matrix
		 * @params Q: Initial process noise covariance matrix
		 * @params R: Initial measurement noise covariance matrix
		 *
		 * @return none
		 */
		ekf_health_t init(Vector7f state_init,
						Matrix7f P_init,
						Matrix7f Q,
						Matrix3f R);

		ekf_health_t predict();

		ekf_health_t update();


	private:
		Vector7f state;
		Vector3f accel_readings;
		Vector3f gyro_readings;
		Matrix7f P;
		Matrix7f Q;
		Matrix3f R;

		/**
		 * @brief Provide skew-symmetric matrix of a quaternion
		 *
		 * Provide a skew-symmetric matrix of a quaternion, to be multiplied with a Vector3f containing rotational rates.
		 *
		 * @param state: State vector which the skew-symmetrix matrix is a function of.
		 * @return none.
		 */
		Matrix4x3f skewQuat(const Eigen::Ref<const Vector4f>& quaternion);
		
		/**
		 * @brief Normalize a quaternion.
		 *
		 * @param quaternion: Quaternion to be normalized.
		 * @return none.
		 */
		void normQuat(Eigen::Ref<Vector4f> quaternion);

		/**
		 * @brief Provide the state function Jacobian df(x)/dt, evaluated at the current time.
		 *
		 * @param state: State vector which the Jacobian is a function of.
		 * @return matrix that represents the state function Jacobian.
		 */
		 Matrix7f getF(const Eigen::Ref<Vector7f> state);

		/**
		 * @brief Provide the measurement function Jacobian dh(x)/dt, evaluated at the current state.
		 *
		 * @param state: State vector wich the Jacobian is a function of.
		 * @return matrix that represents the measurement function Jacobian.
		 */
		Matrix3x7f getH(const Eigen::Ref<Vector7f> state);

		/**
		 * @brief Provide the output of the measurement function, h(x).
		 *
		 * @param state: State vector which the measurement function is a function of.
		 * @return vector h(x) evaluated at x=state
		 */
		Vector3f geth(const Eigen::Ref<Vector7f> state);
		
	
}

#endif

