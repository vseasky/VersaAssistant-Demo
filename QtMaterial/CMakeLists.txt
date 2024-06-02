# -------------------------------------- Cmake版本 --------------------------------------
cmake_minimum_required(VERSION 3.5)

# --------------------------------------- 工程名称 ---------------------------------------
project(QtMaterial LANGUAGES CXX VERSION ${VERSION_SHORT})

# --------------------------------------- 加载Qt库 ---------------------------------------
if (${QT_VERSION_MAJOR})
    message(STATUS "Forced to use Qt version ${QT_VERSION_MAJOR}")
    find_package(QT NAMES Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
else ()
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
endif ()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

if (UNIX AND NOT APPLE)
    include_directories(${Qt${QT_VERSION_MAJOR}Gui_PRIVATE_INCLUDE_DIRS})
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)


if (BUILD_QMATERIAL_STATIC)
    set(CMAKE_STATIC_LIBRARY_SUFFIX "_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
endif ()


# --------------------------------------- 编码格式 ---------------------------------------
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#-------------------------------------- -添加项目- --------------------------------------
FILE(GLOB QMATERIAL_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h*
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.h*)
FILE(GLOB QMATERIAL_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c*
        resources.qrc)

# --------------------------------------- 跨平台 ---------------------------------------
# if (UNIX AND NOT APPLE)
#     set(PROJECT_SOURCE_FILES 
#         linux/FloatingWidgetTitleBar.cpp 
#         ${PROJECT_SOURCE_FILES})
#     set(PROJECT_HEADER_FILES 
#         linux/FloatingWidgetTitleBar.h 
#         ${PROJECT_HEADER_FILES})
# endif()

# STATIC or SHARED 对应静态库 或者 动态库
if (BUILD_QMATERIAL_STATIC)
    add_library(${LIB_NAME_MATERIAL} STATIC
            ${QMATERIAL_HEADER_FILES}
            ${QMATERIAL_SOURCE_FILES})
    target_compile_definitions(${LIB_NAME_MATERIAL} PUBLIC QMATERIAL_STATIC_EXPORT)
else ()
    add_library(${LIB_NAME_MATERIAL} SHARED
            ${QMATERIAL_HEADER_FILES}
            ${QMATERIAL_SOURCE_FILES})
    target_compile_definitions(${LIB_NAME_MATERIAL} PRIVATE QMATERIAL_SHARED_EXPORT)
endif ()


# 起一个别名
add_library(Material::${LIB_NAME_MATERIAL} ALIAS ${LIB_NAME_MATERIAL})

target_link_libraries(${LIB_NAME_MATERIAL} PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets)

# --------------------------------------- 跨平台 ---------------------------------------
if (UNIX AND NOT APPLE)
    if (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
        find_package(X11 REQUIRED)
        target_link_libraries(${LIB_NAME_MATERIAL} PUBLIC X11::xcb)
    else ()
        target_link_libraries(${LIB_NAME_MATERIAL} PUBLIC xcb)
    endif ()
endif ()

# --------------------------------------- 设置目标属性 ---------------------------------------
set_target_properties(${LIB_NAME_MATERIAL} PROPERTIES
        AUTOMOC ON # 启用自动 MOC（Meta-Object Compiler）机制
        AUTORCC ON # 启用自动 RCC（Resource Compiler Collection）机制
        CXX_EXTENSIONS OFF # 禁用 C++ 扩展
        VERSION ${VERSION_SHORT}  # 设置目标的版本号
        EXPORT_NAME ${LIB_NAME_MATERIAL}  # 设置导出名称
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib" # 指定目标库输出的归档文件（静态库）所在的目录
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/lib" # 指定目标库输出的共享库文件（动态库）所在的目录
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PlatformDir}/bin" # 指定可执行文件输出的目录
        )

# --------------------------------------- 指定C++版本 ---------------------------------------
if (QT_VERSION_MAJOR STREQUAL "5")
    set_target_properties(${LIB_NAME_MATERIAL} PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED ON)
elseif (QT_VERSION_MAJOR STREQUAL "6")
    set_target_properties(${LIB_NAME_MATERIAL} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON)
endif ()


# --------------------------------------- 创建配置文件 ---------------------------------------
include(CMakePackageConfigHelpers)

# --------------------------------------- 生成版本文件 ---------------------------------------
write_basic_package_version_file(
        "${LIB_NAME_MATERIAL}ConfigVersion.cmake"
        VERSION ${VERSION_SHORT}
        COMPATIBILITY SameMajorVersion
)

# --------------------------------------- 安装文件 ---------------------------------------
install(FILES ${QMATERIAL_HEADER_FILES}
        DESTINATION include/${LIB_NAME_MATERIAL}
        COMPONENT headers
        )

# install(FILES
#     "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE"
#     "${CMAKE_CURRENT_SOURCE_DIR}/../gnu-lgpl-v2.1.md"
#     DESTINATION license/ads
#     COMPONENT license
# )


# --------------------------------------- 构建到目标库 ---------------------------------------
install(TARGETS ${LIB_NAME_MATERIAL}
        EXPORT materialTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/${LIB_NAME_MATERIAL}
        )

install(EXPORT materialTargets
        FILE materialTargets.cmake
        NAMESPACE Material::
        DESTINATION lib/cmake/${LIB_NAME_MATERIAL}
        )

install(FILES qtMaterialConfig.cmake
        RENAME ${LIB_NAME_MATERIAL}Config.cmake
        DESTINATION lib/cmake/${LIB_NAME_MATERIAL}
        )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME_MATERIAL}ConfigVersion.cmake"
        DESTINATION lib/cmake/${LIB_NAME_MATERIAL}
        )

target_include_directories(${LIB_NAME_MATERIAL}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )
