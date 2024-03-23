idf_component_register(SRCS "main.c"
						"${CMAKE_CURRENT_SOURCE_DIR}/../../icm_20608_driver.c"
						INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
						REQUIRES driver i2c_helper)
 
target_compile_options(${COMPONENT_LIB} PUBLIC --coverage)
target_link_libraries(${COMPONENT_LIB} m --coverage)
