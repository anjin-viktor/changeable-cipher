set(PROJECT keyinfo)

set(SOURCES
	src/main.cpp
)

set(LIBS
	${BUILD_DIR}/libkeyio.a
)

include_directories(
	../../common/
	../../libs/keyio/include
)

add_executable (${PROJECT} ${SOURCES})
target_link_libraries (${PROJECT} ${LIBS} ${Boost_LIBRARIES})
add_dependencies(${PROJECT} keyio)
