cmake_minimum_required(VERSION 3.16)
project(ByteShrink LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(MSVC)
    add_compile_options(/W4 /EHsc)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Qt6 COMPONENTS Widgets Concurrent QUIET)
if(Qt6_FOUND)
    message(STATUS "Configuring with Qt6")
    set(QT_VERSION_MAJOR 6)
    set(QtWidgets_TARGET Qt6::Widgets)
    set(QtConcurrent_TARGET Qt6::Concurrent)
else()
    message(STATUS "Falling back to Qt5")
    find_package(Qt5 COMPONENTS Widgets Concurrent REQUIRED)
    set(QT_VERSION_MAJOR 5)
    set(QtWidgets_TARGET Qt5::Widgets)
    set(QtConcurrent_TARGET Qt5::Concurrent)
endif()

add_library(byteshrink_core
    src/HuffmanCompressor.cpp
    src/HuffmanDecompressor.cpp
    src/HuffmanTree.cpp
)

target_include_directories(byteshrink_core PUBLIC include)
target_compile_features(byteshrink_core PUBLIC cxx_std_17)

add_executable(byteshrink_cli
    main/main.cpp
)

target_link_libraries(byteshrink_cli PRIVATE byteshrink_core)

target_include_directories(byteshrink_cli PRIVATE include)

add_executable(byteshrink_gui
    gui/main.cpp
    gui/MainWindow.cpp
    gui/resources.qrc
)

target_include_directories(byteshrink_gui PRIVATE gui include)

target_link_libraries(byteshrink_gui PRIVATE byteshrink_core ${QtWidgets_TARGET} ${QtConcurrent_TARGET})

# Group targets so IDEs show them clearly
set_target_properties(byteshrink_cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(byteshrink_gui PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
