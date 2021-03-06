set(PROJECT keycreator_tests)
include (CTest)

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)

set(SOURCES
	main.cpp
	DecrKeyParamsParsing.cpp
	KeyCreatorTest.cpp
	Params.cpp
	../../../common/Markerator.cpp
)


set(LIBS
	${BUILD_DIR}/libkeycreator_lib.a
	boolcalc
)

include_directories(
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../src
	${CMAKE_CURRENT_SOURCE_DIR}/../../../common
	${CMAKE_CURRENT_SOURCE_DIR}/../../../externals/boolcalc/include/
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)


ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK) 

add_executable (${PROJECT} ${SOURCES})
target_link_libraries (${PROJECT} ${LIBS} ${Boost_LIBRARIES})
add_dependencies(${PROJECT} keycreator_lib)


add_test (${PROJECT} ${BUILD_DIR}/${PROJECT})

