cmake_minimum_required(VERSION 3.7)
project(scintas)

set(CMAKE_CXX_STANDARD 14)

set(wxWidgets_ROOT_DIR /usr/include/wx-3.0)
set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base stc net REQUIRED)
include(${wxWidgets_USE_FILE})

set(SOURCE_FILES main.cpp Editor.cpp EditorFactory.cpp Config.cpp MessageBox/MessageBox.cpp
        IniParser/IniParser.cpp IniParser/StrTools.cpp execute.cpp
        MyTabArt.cpp
        IPC/MyServer.cpp IPC/CmdStruct.cpp
        IPC/MyClient.cpp IPC/MyConnectionClient.cpp
        IPC/MyConnectionServer.cpp
        ../3rdparty/auimod/framemanager.cpp
        ../3rdparty/auimod/auibook.cpp
        ../3rdparty/auimod/tabmdi.cpp
        ../3rdparty/auimod/tabart.cpp
        ../3rdparty/auimod/tabartmsw.cpp
        ../3rdparty/auimod/dockart.cpp
        ../3rdparty/auimod/floatpane.cpp
        ../3rdparty/auimod/auibar.cpp
        )
add_executable(scintas ${SOURCE_FILES})

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror=return-type")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

target_include_directories(scintas
        PRIVATE
        ../3rdparty/cpp-ipc/include
        )

target_link_directories(scintas
        PRIVATE
        ../3rdparty
        )

target_link_libraries(scintas ${wxWidgets_LIBRARIES} ipc)