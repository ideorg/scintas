cmake_minimum_required(VERSION 3.7)
project(scintas)

set(CMAKE_CXX_STANDARD 14)

if(UNIX)
set(wxWidgets_ROOT_DIR /usr/include/wx-3.1)
elseif(WIN32)
set(wxWidgets_ROOT_DIR c:/wxWidgets-3.1.5)
    set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/vc_x64_lib)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(wxWidgets_CONFIGURATION vc_mswud_x64)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(wxWidgets_CONFIGURATION vc_mswu_x64)
    endif()
endif()
find_package(wxWidgets COMPONENTS core base stc net aui REQUIRED)
include("${wxWidgets_USE_FILE}")
message(STATUS "foo include dir: ${wxWidgets_LIBRARIES}")

set(SOURCE_FILES main.cpp Editor.cpp EditorFactory.cpp Config.cpp MessageBox/MessageBox.cpp
        IniParser/IniParser.cpp IniParser/StrTools.cpp execute.cpp
        MyTabArt.cpp
        IPC/MyServer.cpp IPC/CmdStruct.cpp
        IPC/MyClient.cpp IPC/MyConnectionClient.cpp
        IPC/MyConnectionServer.cpp
        MessageBox/MessageBox.cpp
        ../3rdparty/auimod/framemanager.cpp
        ../3rdparty/auimod/auibook.cpp
        ../3rdparty/auimod/tabmdi.cpp
        ../3rdparty/auimod/tabart.cpp
        ../3rdparty/auimod/tabartmsw.cpp
        ../3rdparty/auimod/dockart.cpp
        ../3rdparty/auimod/floatpane.cpp
        ../3rdparty/auimod/auibar.cpp
        compat.h Screen.cpp Screen.h)
add_executable(scintas ${SOURCE_FILES})

if(CMAKE_COMPILER_IS_GNUCXX)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror=return-type")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
else()
add_definitions(-DUSE64bit=true)
endif()
add_definitions(-DwxUSE_UNICODE=1)

target_include_directories(scintas
        PRIVATE
        ../3rdparty/cpp-ipc/include
        )

target_link_directories(scintas
        PRIVATE
        ../3rdparty
        )

target_link_libraries(scintas ${wxWidgets_LIBRARIES} X11)
