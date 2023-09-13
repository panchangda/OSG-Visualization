cmake_minimum_required(VERSION 3.20)
project("OSG_VIS")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(OSGB_DIR "D:\\vscodes\\osg\\OpenSceneGraph-3.6.5-VC2022-64-Debug")	

file(GLOB 
	SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

add_executable(OSG_VIS ${SOURCES})

target_link_libraries(OSG_VIS  ${OSGB_DIR}/lib/*)
target_include_directories(OSG_VIS PUBLIC ${OSGB_DIR}/include	)

# avoid error: ".dll not find" while executing target 
# set_property(TARGET OSG_VIS PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${OSGB_DIR}/bin)
