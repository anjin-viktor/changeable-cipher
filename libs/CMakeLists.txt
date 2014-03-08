if(CMAKE_COMPILER_IS_GNUCXX )
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++0x")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++0x")
endif()

enable_testing()
add_subdirectory(keycreator_lib/)
add_subdirectory(keyio/)
