cmake_minimum_required(VERSION 2.8)

set (BUILD_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/build
)

if(NOT IS_DIRECTORY ${BUILD_DIR})
	file (MAKE_DIRECTORY
		${BUILD_DIR}
	)
endif()

set (EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})
set (LIBRARY_OUTPUT_PATH ${BUILD_DIR})

add_subdirectory(libs/)
add_subdirectory(utils/)

set (BOOLCALC_BUILD_AS_PART_OF_PROJECT true)
add_subdirectory(externals/boolcalc)

enable_testing()
