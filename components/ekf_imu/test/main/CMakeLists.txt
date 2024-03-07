idf_component_register(SRCS "test_mat_utils.c" "test_ekf.c" "test_main.c"
						"${CMAKE_CURRENT_SOURCE_DIR}/../../array_utils.c"
						"${CMAKE_CURRENT_SOURCE_DIR}/../../mat_utils.c"
						"${CMAKE_CURRENT_SOURCE_DIR}/../../ekf_imu.c"
						INCLUDE_DIRS "."
						"${CMAKE_CURRENT_SOURCE_DIR}/../.."
						"${CMAKE_CURRENT_SOURCE_DIR}/../../include"
						REQUIRES unity )

					target_compile_options(${COMPONENT_LIB} PUBLIC --coverage)
					target_link_libraries(${COMPONENT_LIB} m --coverage)

