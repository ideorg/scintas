cmake_minimum_required(VERSION 3.7)
project(MessageBox)

set(CMAKE_CXX_STANDARD 11)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror=return-type")

set(wxWidgets_ROOT_DIR /usr/include/wx-3.0)
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

set(SOURCE_FILES main.cpp MessageBox.cpp)
add_executable(MessageBox ${SOURCE_FILES})

target_link_libraries(MessageBox ${wxWidgets_LIBRARIES})