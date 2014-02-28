set(PROJECT keycreator_lib)

project(${PROJECT})

set(SOURCES
	DecrKeyParams.cpp
	KeyStream.cpp
	LFSRParams.cpp
	KeyCreator.cpp
	DisForm.cpp
	../../auxiliary/LFSR.cpp
	../../auxiliary/Markerator.cpp
)

find_package(Boost COMPONENTS REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
	../../externals/boolcalc/include/
	../../auxiliary/
)

add_library(${PROJECT} ${SOURCES}) 
target_link_libraries(${PROJECT} ${LIBS})
