idf_component_register(SRCS "test_mat_utils.c"
						"test_ekf_imu.c"
						"${CMAKE_CURRENT_SOURCE_DIR}/../../mat_utils.c"
						INCLUDE_DIRS "."
						"${CMAKE_CURRENT_SOURCE_DIR}/../.."
						"${CMAKE_CURRENT_SOURCE_DIR}/../../include"
						REQUIRES unity driver )

					target_compile_options(${COMPONENT_LIB} PUBLIC --coverage)
					target_link_libraries(${COMPONENT_LIB} m --coverage)

