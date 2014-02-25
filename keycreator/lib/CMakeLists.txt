set(PROJECT keycreator_lib)

project(${PROJECT})

set(SOURCES
	DecrKeyParams.cpp
)

find_package(Boost COMPONENTS REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT} ${SOURCES}) 
target_link_libraries(${PROJECT} ${LIBS})
