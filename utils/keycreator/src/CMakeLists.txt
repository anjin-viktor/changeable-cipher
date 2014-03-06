set(PROJECT keycreator)

set(SOURCES
	main.cpp
)

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)

set(LIBS
	${BUILD_DIR}/libkeyio.a
	${BUILD_DIR}/libkeycreator_lib.a
	${Boost_LIBRARIES}
	boolcalc
)

include_directories(
	../../../common/
	../../../libs/keyio/include
	../../../libs/keycreator_lib/include
	${Boost_INCLUDE_DIRS}
)

add_executable (${PROJECT} ${SOURCES})
target_link_libraries (${PROJECT} ${LIBS} ${Boost_LIBRARIES})

add_dependencies(${PROJECT} keyio)
add_dependencies(${PROJECT} keycreator_lib)
