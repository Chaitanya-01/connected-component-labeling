cmake_minimum_required(VERSION 3.22)
project(connected-component-labeling)

set(CMAKE_CXX_STANDARD 17)

#  generate compiple commands file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

link_directories()
# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/results/lib)
# set executables folder
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/results/bin)

add_executable(ccl src/ccl.cpp)
target_link_libraries(ccl ${OpenCV_LIBS})