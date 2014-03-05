set(PROJECT keyio_tests)
include (CTest)

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)

set(SOURCES
	main.cpp
	Params.cpp
	KeyIOTest.cpp
)


set(LIBS
	${BUILD_DIR}/libkeyio.a
	boolcalc
)

include_directories(
	../../../common/
	../include/
	${Boost_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)


ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK) 

add_executable (${PROJECT} ${SOURCES})
target_link_libraries (${PROJECT} ${LIBS} ${Boost_LIBRARIES})
add_dependencies(${PROJECT} keyio)


add_test (${PROJECT} ${BUILD_DIR}/${PROJECT})

