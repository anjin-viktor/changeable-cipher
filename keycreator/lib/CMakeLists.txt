set(PROJECT keycreator_lib)

project(${PROJECT})

set(SOURCES
	DecrKeyParams.cpp
	KeyStream.cpp
	LFSRParams.cpp
	../../auxiliary/LFSR.h
)

set(LIBS
	boolcalc
)

find_package(Boost COMPONENTS REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
	../../externals/boolcalc/include/
	../../auxiliary/
)

if(CMAKE_COMPILER_IS_GNUCXX )
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif()

add_library(${PROJECT} ${SOURCES}) 
target_link_libraries(${PROJECT} ${LIBS})
