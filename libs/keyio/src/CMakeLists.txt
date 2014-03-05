set(PROJECT keyio)

project(${PROJECT})

set(SOURCES
	KeyIO.cpp
	BitstreamReader.cpp
	BitstreamWriter.cpp
)

find_package(Boost COMPONENTS REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
	../../../common/
	../include/
)

add_library(${PROJECT} ${SOURCES}) 
target_link_libraries(${PROJECT} ${LIBS})
