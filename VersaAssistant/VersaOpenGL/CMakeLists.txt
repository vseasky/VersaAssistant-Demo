## -------------------------------------- Cmake版本 --------------------------------------
#cmake_minimum_required(VERSION 3.5)
## --------------------------------------- 工程名称 ---------------------------------------
#project(VersaAssistant LANGUAGES CXX VERSION ${VERSION_SHORT})
## --------------------------------------- 加载Qt库 ---------------------------------------
#if (${QT_VERSION_MAJOR})
#    message(STATUS "Forced to use Qt version ${QT_VERSION_MAJOR}")
#    find_package(QT NAMES Qt${QT_VERSION_MAJOR} COMPONENTS Core  PrintSupport REQUIRED)
#else()
#    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
#endif()
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Quick QuickWidgets PrintSupport REQUIRED)
#
#if (UNIX AND NOT APPLE)
#    include_directories(${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
#endif()
#
#
#if(WIN32 AND QT_VERSION_MAJOR LESS 6)
#    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS AxContainer REQUIRED)
#endif()
#
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#
## --------------------------------------- 编码格式 ---------------------------------------
#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
##-------------------------------------- -添加项目- --------------------------------------
#FILE(GLOB PROJECT_HEADER_FILES
#        ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
##        ${CMAKE_CURRENT_SOURCE_DIR}/assimp/*.h*
#)
#FILE(GLOB PROJECT_SOURCE_FILES
#        ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
##        ${CMAKE_CURRENT_SOURCE_DIR}/assimp/*.c*
#        resources.qrc
#)
#
#
#if(ANDROID)
#    add_library(${VersaAssistantApp} SHARED
#            ${PROJECT_HEADER_FILES}
#            ${PROJECT_SOURCE_FILES})
#else()
#    add_executable(${VersaAssistantApp}
#            ${PROJECT_HEADER_FILES}
#            ${PROJECT_SOURCE_FILES})
#endif()
#
#target_include_directories(${VersaAssistantApp} PRIVATE
##        "${CMAKE_CURRENT_SOURCE_DIR}/VseraQCustomplot"
##        "${CMAKE_CURRENT_SOURCE_DIR}/../QtMaterial"
##        "${CMAKE_CURRENT_SOURCE_DIR}/../QtMaterial/lib"
##        "${CMAKE_CURRENT_SOURCE_DIR}/../QtAdvancedDocking"
##        "${CMAKE_CURRENT_SOURCE_DIR}/assimp"
#        "${CMAKE_CURRENT_SOURCE_DIR}/../assimp/include"
#)
#
##target_include_directories(${VersaAssistantApp} PRIVATE
##        "${CMAKE_CURRENT_SOURCE_DIR}/../assimp/include"
##)
#
#link_directories("${CMAKE_BINARY_DIR}/${PlatformDir}/bin")
#
#target_link_libraries(${VersaAssistantApp} PUBLIC
#        Qt${QT_VERSION_MAJOR}::Core
#        Qt${QT_VERSION_MAJOR}::Gui
#        Qt${QT_VERSION_MAJOR}::Widgets
#        Qt${QT_VERSION_MAJOR}::Quick
#        Qt${QT_VERSION_MAJOR}::QuickWidgets
#        Qt${QT_VERSION_MAJOR}::PrintSupport)
#
##if(WIN32 AND QT_VERSION_MAJOR LESS 6)
##    target_link_libraries(${VersaAssistantApp} PUBLIC
##            Qt${QT_VERSION_MAJOR}::AxContainer)
##endif()
#
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
#
#target_link_libraries(${VersaAssistantApp} PRIVATE
##        ${LIB_NAME_ADS}
##        ${LIB_NAME_MATERIAL}
#         ${LIB_NAME_ASSIMP}
#)
#
#set_target_properties(${VersaAssistantApp} PROPERTIES
#        AUTOMOC ON
#        AUTORCC ON
#        AUTOUIC ON
#        CXX_STANDARD 14
#        CXX_STANDARD_REQUIRED ON
#        CXX_EXTENSIONS OFF
#        VERSION ${VERSION_SHORT}
#        EXPORT_NAME "${VersaAssistantApp}"
#        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib"
#        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib"
#        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/bin"
#)
#
#
#
