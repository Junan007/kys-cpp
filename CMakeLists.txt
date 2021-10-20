cmake_minimum_required(VERSION 2.6)

project(kys_c)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR/bin})

include_directories(${PROJECT_SOURCE_DIR}/include 
					${PROJECT_SOURCE_DIR}/common 
					${PROJECT_SOURCE_DIR}/others 
					${PROJECT_SOURCE_DIR}/local/include
					)

if (APPLE)
	set(LIB_PATH mac)
else()
	set(LIB_PATH x86)
endif()

# link_directories(${PROJECT_SOURCE_DIR}/lib/${LIB_PATH})
link_directories(/usr/local/lib
				 ${PROJECT_SOURCE_DIR}/lib/${LIB_PATH})

add_subdirectory(src)