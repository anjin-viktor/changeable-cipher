set(PROJECT keycreator_lib)

project(${PROJECT})

set(SOURCES
	DecrKeyParams.cpp
	KeyStream.cpp
	KeyCreator.cpp
	KeyCreatorImpl.cpp
	../../../common/DisForm.cpp
	../../../common/LFSR.cpp
)

find_package(Boost COMPONENTS REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
	../../../externals/boolcalc/include/
	../../../common/
	../include/

)

add_library(${PROJECT} ${SOURCES}) 
target_link_libraries(${PROJECT} ${LIBS})
