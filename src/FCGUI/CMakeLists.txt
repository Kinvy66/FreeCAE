
# Cmake的命令不区分大小写，例如message，set等命令；但Cmake的变量区分大小写
# 为统一风格，本项目的Cmake命令全部采用小写，变量全部采用大写加下划线组合。
# FCGUI 库

cmake_minimum_required(VERSION 3.12)

fcmacro_lib_setting(
    "FCGUI"
    "FCGUI | https://github.com/kinvy66"
    0
    0
    1
)

########################################################
# Qt
########################################################
set(FC_MIN_QT_VERSION 5.14)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} ${FC_MIN_QT_VERSION} COMPONENTS
    Core
    Gui
    Widgets
    Xml
    REQUIRED
)


########################################################
# 文件加载
########################################################
file(GLOB FC_LIB_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB FC_LIB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB FC_LIB_UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")
file(GLOB FC_LIB_QT_RC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc")

file(GLOB FC_GEO_LIB_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/FCGeometryProperty/*.h")
file(GLOB FC_GEO_LIB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/FCGeometryProperty/*.cpp")
file(GLOB FC_GEO_LIB_UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/FCGeometryProperty/*.ui")

add_library(${FC_LIB_NAME} SHARED
            ${FC_LIB_HEADER_FILES}
            ${FC_LIB_SOURCE_FILES}
            ${FC_LIB_UI_FILES}
            ${FC_LIB_QT_RC_FILES}
            
            ${FC_GEO_LIB_HEADER_FILES}
            ${FC_GEO_LIB_SOURCE_FILES}
            ${FC_GEO_LIB_UI_FILES}
)	
# 构建库定义的宏,必须为Private
target_compile_definitions(${FC_LIB_NAME} PRIVATE FCGUI_BUILDLIB)
# 定义别名让DAWorkbench::${DA_LIB_NAME}也能获取到
add_library(${FC_PROJECT_NAME}::${FC_LIB_NAME} ALIAS ${FC_LIB_NAME})
########################################################
# 依赖链接
########################################################
target_link_libraries(${FC_LIB_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
)
message(STATUS "${FC_LIB_NAME} Qt${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")

# -------------link 3rdparty spdlog--------------------------
# FCMessageHandler依赖spdlog
# spdlog是header only
# fcmacro_import_spdlog(${FC_LIB_NAME})

# 依赖模块（FCGeometryInterface 供 FCCubeInfoWidget 绑定 FCGeoModelBox）
find_package(${FC_PROJECT_NAME} COMPONENTS 
    FCUtils
    FCInterface
    FCGeometryInterface
    FCMessageHandler
    FCRenderWindowVTK
)
if(${FC_PROJECT_NAME}_FOUND)
    message(STATUS "  |-linked ${FC_PROJECT_NAME}::FCUtils")
    message(STATUS "  |-linked ${FC_PROJECT_NAME}::FCGeometryInterface")
endif()
target_link_libraries(${FC_LIB_NAME} PUBLIC
    ${FC_PROJECT_NAME}::FCUtils
    ${FC_PROJECT_NAME}::FCInterface
    ${FC_PROJECT_NAME}::FCGeometryInterface
    ${FC_PROJECT_NAME}::FCMessageHandler
    ${FC_PROJECT_NAME}::FCRenderWindowVTK
)

########################################################
# 设置通用库属性
########################################################
fcmacro_set_lib_properties(${FC_LIB_NAME} ${FC_LIB_VERSION})

target_include_directories(${FC_LIB_NAME} PUBLIC
    $<INSTALL_INTERFACE:include/${FC_PROJECT_NAME}/${FC_LIB_NAME}/FCGeometryProperty>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/FCGeometryProperty>
)


########################################################
# 安装
########################################################
fcmacro_lib_install()

install(FILES
    ${FC_LIB_HEADER_FILES}
    DESTINATION include/${FC_PROJECT_NAME}/${FC_LIB_NAME}
    COMPONENT headers
)

install(FILES
    ${FC_GEO_LIB_HEADER_FILES}
    DESTINATION include/${FC_PROJECT_NAME}/${FC_LIB_NAME}/FCGeometryProperty
    COMPONENT headers
)
