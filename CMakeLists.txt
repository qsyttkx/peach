cmake_minimum_required(VERSION 3.16)

project(peach VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick QuickControls2 REQUIRED)

if(WIN32)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
    add_compile_options(/utf-8)
    set(VCPKG_CMAKE "D:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE STRING "vcpkg.cmake的位置")
    include(${VCPKG_CMAKE})
    find_package(FFMPEG 4.4 REQUIRED)
    include_directories(${FFMPEG_INCLUDE_DIRS})
    link_directories(${FFMPEG_LIBRARY_DIRS})
endif()

qt_add_executable(appPeach
    main.cpp MainViewModel.h MainViewModel.cpp PacketHistogram.h PacketHistogram.cpp
)

qt_add_qml_module(appPeach
    URI Peach
    VERSION 1.0
    QML_FILES main.qml 
)

set_target_properties(appPeach PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER peach.qsyttkx.xyz
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appPeach
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appPeach
    PRIVATE Qt6::Quick Qt6::QuickControls2 avformat avcodec)
