set(PROJECT cipher)

set(SOURCES
	main.cpp
)

find_package(Boost COMPONENTS program_options system REQUIRED)
find_package(OpenSSL REQUIRED)

set(LIBS
	${BUILD_DIR}/libkeyio.a
	${BUILD_DIR}/libkeycreator_lib.a
	boolcalc
	${Boost_LIBRARIES}
	${OPENSSL_LIBRARIES}
)

include_directories(
	../../../common/
	../../../libs/keyio/include
	../../../libs/keycreator_lib/include
	../../../externals/boolcalc/include
	${Boost_INCLUDE_DIRS}
	${OPENSSL_INCLUDE_DIR}
)

add_executable (${PROJECT} ${SOURCES})
target_link_libraries (${PROJECT} ${LIBS} ${Boost_LIBRARIES})

add_dependencies(${PROJECT} keyio)
add_dependencies(${PROJECT} keycreator_lib)
