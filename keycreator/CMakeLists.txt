if(CMAKE_COMPILER_IS_GNUCXX )
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif()

enable_testing()
add_subdirectory(lib/)
add_subdirectory(tests/)
