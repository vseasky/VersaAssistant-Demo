# -------------------------------------- Cmake版本 --------------------------------------
cmake_minimum_required(VERSION 3.5)

# --------------------------------------- 工程名称 ---------------------------------------
project(QtAdvancedDockingSystem LANGUAGES CXX VERSION ${VERSION_SHORT})

# --------------------------------------- 加载Qt库 ---------------------------------------
if (${QT_VERSION_MAJOR})
  message(STATUS "Forced to use Qt version ${QT_VERSION_MAJOR}")
  find_package(QT NAMES Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
else()
  find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
endif()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

if (UNIX AND NOT APPLE)
    include_directories(${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(BUILD_ADS_STATIC)
    set(CMAKE_STATIC_LIBRARY_SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif()

# --------------------------------------- 编码格式 ---------------------------------------
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#-------------------------------------- -添加项目- --------------------------------------
FILE(GLOB ads_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h*)
FILE(GLOB ads_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
        ads.qrc)

# --------------------------------------- 跨平台 ---------------------------------------
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
if (UNIX AND NOT APPLE)
    set(ads_SRCS linux/FloatingWidgetTitleBar.cpp ${ads_SRCS})
    set(ads_HEADERS linux/FloatingWidgetTitleBar.h ${ads_HEADERS})
endif()

# STATIC or SHARED 对应静态库 或者 动态库
if(BUILD_ADS_STATIC)
    add_library(${LIB_NAME_ADS} STATIC
    ${ads_SRCS} 
    ${ads_HEADERS})
    target_compile_definitions(${LIB_NAME_ADS} PUBLIC ADS_STATIC_EXPORT)
else()
    add_library( ${LIB_NAME_ADS} SHARED 
    ${ads_SRCS} 
    ${ads_HEADERS})
    target_compile_definitions( ${LIB_NAME_ADS} PRIVATE ADS_SHARED_EXPORT)
endif()

# 起一个别名
add_library(ads::${LIB_NAME_ADS} ALIAS ${LIB_NAME_ADS})

target_link_libraries(${LIB_NAME_ADS} PUBLIC 
                    Qt${QT_VERSION_MAJOR}::Core 
                    Qt${QT_VERSION_MAJOR}::Gui 
                    Qt${QT_VERSION_MAJOR}::Widgets)

# --------------------------------------- 跨平台 ---------------------------------------
if (UNIX AND NOT APPLE)
  if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
      find_package(X11 REQUIRED)
      target_link_libraries(${LIB_NAME_ADS} PUBLIC X11::xcb)
  else()
    target_link_libraries(${LIB_NAME_ADS} PUBLIC xcb)
  endif()
endif()

# --------------------------------------- 设置目标属性 ---------------------------------------
set_target_properties(${LIB_NAME_ADS} PROPERTIES
    AUTOMOC ON
    AUTORCC ON
    CXX_EXTENSIONS OFF
    VERSION ${VERSION_SHORT}
    EXPORT_NAME ${LIB_NAME_ADS}
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/bin"
)

# --------------------------------------- 指定C++版本 ---------------------------------------
if(QT_VERSION_MAJOR STREQUAL "5")
    set_target_properties(${LIB_NAME_ADS} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)
elseif(QT_VERSION_MAJOR STREQUAL "6")
    set_target_properties(${LIB_NAME_ADS} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON)
endif()

# --------------------------------------- 创建配置文件 ---------------------------------------
include(CMakePackageConfigHelpers)

# --------------------------------------- 生成版本文件 ---------------------------------------
write_basic_package_version_file(
    "${LIB_NAME_ADS}ConfigVersion.cmake"
    VERSION ${VERSION_SHORT}
    COMPATIBILITY SameMajorVersion
)

# --------------------------------------- 安装文件 ---------------------------------------
install(FILES ${ads_HEADERS}
    DESTINATION include/${LIB_NAME_ADS}
    COMPONENT headers
)

# install(FILES
#     "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE"
#     "${CMAKE_CURRENT_SOURCE_DIR}/../gnu-lgpl-v2.1.md"
#     DESTINATION license/ads
#     COMPONENT license
# )

install(TARGETS ${LIB_NAME_ADS}
    EXPORT adsTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include/${LIB_NAME_ADS}
)

install(EXPORT adsTargets
    FILE adsTargets.cmake
    NAMESPACE ads::
    DESTINATION lib/cmake/${LIB_NAME_ADS}
)
install(FILES qtadvanceddockingConfig.cmake RENAME ${LIB_NAME_ADS}Config.cmake
    DESTINATION lib/cmake/${LIB_NAME_ADS}
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME_ADS}ConfigVersion.cmake"
    DESTINATION lib/cmake/${LIB_NAME_ADS}
)

target_include_directories(${LIB_NAME_ADS} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
