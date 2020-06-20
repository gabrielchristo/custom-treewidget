cmake_minimum_required(VERSION 3.5)

project(CustomTreeWidget LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

set(HEADER
    MainWindow.h
    GenericStruct.h
    CustomStruct.h
    TreeUtils.h
)

set(SOURCE
    Main.cpp
    MainWindow.cpp
    TreeUtils.cpp
)

set(UI
    MainWindow.ui
)

qt5_wrap_cpp(QtMoc ${HEADER})
qt5_wrap_ui(QtUi ${UI})

add_executable(CustomTreeWidget
    ${HEADER}
    ${SOURCE}
    ${UI}
    ${QtMoc}
    ${QtUi}
)

target_link_libraries(CustomTreeWidget PRIVATE Qt5::Widgets Qt5::Core)
