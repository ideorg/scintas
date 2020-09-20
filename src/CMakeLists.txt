cmake_minimum_required(VERSION 3.7)
project(scintas)

set(CMAKE_CXX_STANDARD 14)

set(wxWidgets_ROOT_DIR /usr/include/wx-3.0)
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base aui stc REQUIRED)
include(${wxWidgets_USE_FILE})

set(SOURCE_FILES main.cpp)
add_executable(scintas ${SOURCE_FILES})

target_link_libraries(scintas ${wxWidgets_LIBRARIES})