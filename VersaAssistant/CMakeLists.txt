# -------------------------------------- Cmake版本 --------------------------------------
cmake_minimum_required(VERSION 3.2)

# --------------------------------------- 工程名称 ---------------------------------------
project(VersaAssistant LANGUAGES CXX VERSION ${VERSION_SHORT})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# --------------------------------------- 配置文件 ---------------------------------------
# 获取当前日期
string(TIMESTAMP CURRENT_DATE "%Y-%m-%d")
file(GENERATE
        OUTPUT ${CMAKE_BINARY_DIR}/VersaAssistant/include/VersaAppVersionConfig.h.tmp
        CONTENT "\
        #ifndef VERSA_APP_VERSION_CONFIG_H\n\
        #define VERSA_APP_VERSION_CONFIG_H\n\
        \n\
        #define VERSA_ASSISTANT_NAME \"${VersaAssistantName}\"\n\
        #define VERSA_ASSISTANT_VERSION \"${VersaAssistantVersion}\"\n\
        #define VERSA_ASSISTANT_APP \"${VersaAssistantApp}\"\n\
        #define VERSA_BUILD_PLATFOR \"${BuildPlatfor}\"\n\
        #define VERSA_BUILD_DATE \"${CURRENT_DATE}\"\n\
        #define VERSA_BUILD_TOOL \"Qt${QT_VERSION}\"\n\
        \n\
        #define LIB_NAME_ADS \"${LIB_NAME_ADS}\"\n\
        #define LIB_NAME_FREEGULT \"${LIB_NAME_FREEGULT}\"\n\
        #define LIB_NAME_ASSIMP \"${LIB_NAME_ASSIMP}\"\n\
        #define LIB_NAME_LIBUSB \"${LIB_NAME_LIBUSB}\"\n\
        \n\
        #endif\n\
        "
)

set(QT_DEBUG_PLUGINS ON)
set(DBUILD_SHARED_LIBS ON)

# 设置源文件路径
get_filename_component(src_file "${CMAKE_BINARY_DIR}/VersaAssistant/include/VersaAppVersionConfig.h.tmp" ABSOLUTE)
# 设置目标文件路径
get_filename_component(dest_file "${CMAKE_BINARY_DIR}/VersaAssistant/include/VersaAppVersionConfig.h" ABSOLUTE)
# 复制文件并覆盖
file(REMOVE ${dest_file})

execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${src_file}
        ${dest_file}
)

file(COPY "${OpenSSL_DIR}" DESTINATION "${CMAKE_BINARY_DIR}/${PlatformDir}/bin/")
# --------------------------------------- 加载Qt库 ---------------------------------------

if (${Qt_VERSION_MAJOR})
    message(STATUS "Forced to use Qt version ${QT_VERSION_MAJOR}")
    find_package(QT NAMES Qt${QT_VERSION_MAJOR} COMPONENTS Core OpenGL Network xml PrintSupport REQUIRED)
else ()
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
endif ()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Quick QuickWidgets SerialPort OpenGL Network xml Svg PrintSupport REQUIRED)

if (UNIX AND NOT APPLE)
    include_directories(${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
endif ()

if (WIN32 AND QT_VERSION_MAJOR LESS 6)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS AxContainer REQUIRED)
endif ()

#set_target_properties(${LIB_NAME_ADS} PROPERTIES AUTOMOC ON)


add_definitions(-DQCUSTOMPLOT_USE_OPENGL)

# --------------------------------------- 编码格式 ---------------------------------------
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
#-------------------------------------- -添加项目- --------------------------------------
FILE(GLOB PROJECT_HEADER_FILES
        ${CMAKE_BINARY_DIR}/assimp/include/assimp/*.h*
        ${CMAKE_BINARY_DIR}/VersaAssistant/include/*.h*
        #        ${CMAKE_CURRENT_SOURCE_DIR}/../assimp/include/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/Frameless/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaQCustomplot/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaOpenGL/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaSerial/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaInternet/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaLibusb/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaProtocol/Protocol/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaProtocol/Protocol/crc/*.h*
)
FILE(GLOB PROJECT_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/Frameless/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaQCustomplot/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaOpenGL/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaSerial/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaInternet/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaLibusb/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaProtocol/Protocol/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaProtocol/Protocol/crc/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/VersaStyle.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/font/JetBrainsMono.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/font/Roboto.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/Icon/MingCute.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/Icon/IconPark.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/VersaOpenGL/resources.qrc
        ../QtMaterial/resources.qrc
        ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc
)

if (ANDROID)
    add_library(${VersaAssistantName} SHARED
            ${PROJECT_HEADER_FILES}
            ${PROJECT_SOURCE_FILES})
else ()
    add_executable(${VersaAssistantName}
            ${PROJECT_HEADER_FILES}
            ${PROJECT_SOURCE_FILES})

    #    add_executable(${VersaAssistantName} WIN32
    #            ${PROJECT_HEADER_FILES}
    #            ${PROJECT_SOURCE_FILES})
endif ()

target_include_directories(${VersaAssistantName} PRIVATE
        "${CMAKE_BINARY_DIR}/assimp/include/assimp"
        "${CMAKE_BINARY_DIR}/VersaAssistant/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaQCustomplot"
        "${CMAKE_CURRENT_SOURCE_DIR}/Frameless"
        "${CMAKE_CURRENT_SOURCE_DIR}/../QtAdvancedDocking"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaWidget/"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaOpenGL/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../assimp/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaSerial"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaInternet"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaLibusb"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaProtocol/Protocol"
        "${CMAKE_CURRENT_SOURCE_DIR}/VersaCommunication/VersaProtocol/Protocol/crc"
        "${LibusbPath}/include/libusb-1.0"
)

target_link_libraries(${VersaAssistantName} PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickWidgets
        Qt${QT_VERSION_MAJOR}::SerialPort
        Qt${QT_VERSION_MAJOR}::OpenGL
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Svg
)


if (WIN32 AND QT_VERSION_MAJOR LESS 6)
    target_link_libraries(${VersaAssistantName} PUBLIC
            Qt${QT_VERSION_MAJOR}::AxContainer)
endif ()


target_link_libraries(${VersaAssistantName} PRIVATE
        ${LIB_NAME_ADS}
        ${LIB_NAME_ASSIMP}
        "${CMAKE_BINARY_DIR}/${PlatformDir}/lib/libusb-1.0.a"
        ${LIB_NAME_FREEGULT}
)

set_target_properties(${VersaAssistantName} PROPERTIES
        AUTOMOC ON
        AUTORCC ON
        AUTOUIC ON
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION ${VERSION_SHORT}
        EXPORT_NAME "${VersaAssistantName}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/bin"
)


