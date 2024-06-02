cmake_minimum_required(VERSION 3.2)

set(APP_VERSION 0.0.9)
# 设置软件版本，同步git版本
if (NOT APP_VERSION)
    set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
    include(GetGitRevisionDescription)
    git_describe(GitTagVersion --tags)
    # 正则替换，提取版本号
    string(REGEX REPLACE "^([0-9]+)//..*" "//1" VERSION_MAJOR "${GitTagVersion}")
    string(REGEX REPLACE "^[0-9]+//.([0-9]+).*" "//1" VERSION_MINOR "${GitTagVersion}")
    string(REGEX REPLACE "^[0-9]+//.[0-9]+//.([0-9]+).*" "//1" VERSION_PATCH "${GitTagVersion}")
    set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
else ()
    string(REGEX MATCHALL "[/.]" VERSION_DOT_MATCHES ${APP_VERSION})
    list(LENGTH VERSION_DOT_MATCHES VERSION_DOT_COUNT)
    if (VERSION_DOT_COUNT EQUAL 2)
        set(VERSION_SHORT ${APP_VERSION})
    else ()
        message(FATAL_ERROR "APP_VERSION must be in major.minor.patch format, e.g. 3.8.1. Got ${APP_VERSION}")
    endif ()
endif ()

# 应用名称
project(VersaDebugAssistantProject LANGUAGES CXX VERSION ${VERSION_SHORT})
find_package(Qt5 COMPONENTS Core REQUIRED)

set(APP_CMAKE_SUB_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(AppOutputDir "build_out")

set(QT_VERSION "${Qt5_VERSION}")
set(QT_VERSION_MAJOR "${Qt5_VERSION_MAJOR}")

set(VersaAssistantName "VersaAssistant")
set(VersaAssistantVersion "${VERSION_SHORT}")
set(VersaAssistantApp "${VersaAssistantName}-${VersaAssistantVersion}")

set(LIB_NAME_ADS "qt${QT_VERSION_MAJOR}advanceddocking")
set(LIB_NAME_FREEGULT "freeglut")
set(LIB_NAME_ASSIMP "assimp")
set(LIB_NAME_LIBUSB "libusb")


option(BUILD_ADS_STATIC "Build the ads static library" OFF)
option(BUILD_QMATERIAL_STATIC "Build the material static library" OFF)
option(BUILD_EXAMPLES "Build the examples" ON)

if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(CMAKE_PREFIX_PATH "D:/SoftwareTools/Qt/Qt5.14.2/5.14.2/mingw73_32")
    set(PATH "D:/SoftwareTools/Qt/Qt5.14.2/5.14.2/mingw73_32/bin")
    set(PlatformDir "../platform/x86")
    set(LibusbPath "${CMAKE_CURRENT_SOURCE_DIR}/VersaAssistant/VersaCommunication/VersaLibusb/libusb/libusb-MinGW-Win32")
    set(BuildPlatfor "MinGW-x86")
    set(OpenSSL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OpenSSL/Win32/")
else ()
    set(CMAKE_PREFIX_PATH "D:/SoftwareTools/Qt/Qt5.14.2/5.14.2/mingw73_64")
    set(PATH "D:/SoftwareTools/Qt/Qt5.14.2/5.14.2/mingw73_64/bin")
    set(PlatformDir "../platform/x64")
    set(LibusbPath "${CMAKE_CURRENT_SOURCE_DIR}/VersaAssistant/VersaCommunication/VersaLibusb/libusb/libusb-MinGW-x64")
    set(BuildPlatfor "MinGW-x64")
    set(OpenSSL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/OpenSSL/Win64/")
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/bin")

file(COPY "${LibusbPath}/bin/" DESTINATION "${CMAKE_BINARY_DIR}/${PlatformDir}/bin/")
file(COPY "${LibusbPath}/lib/" DESTINATION "${CMAKE_BINARY_DIR}/${PlatformDir}/lib/")

add_subdirectory(assimp)
add_subdirectory(QtAdvancedDocking)
add_subdirectory(freeglut)
add_subdirectory(VersaAssistant)
