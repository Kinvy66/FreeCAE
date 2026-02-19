# FCGeometryEntity 库 - 运行期几何实体层（Domain/Boundary/Edge/Point）
cmake_minimum_required(VERSION 3.12)

fcmacro_lib_setting(
    "FCGeometryEntity"
    "FCGeometryEntity | https://github.com/kinvy66"
    0
    0
    1
)

set(FC_MIN_QT_VERSION 5.14)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} ${FC_MIN_QT_VERSION} COMPONENTS Core Gui REQUIRED)

file(GLOB FC_LIB_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB FC_LIB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${FC_LIB_NAME} SHARED
    ${FC_LIB_HEADER_FILES}
    ${FC_LIB_SOURCE_FILES}
)
target_compile_definitions(${FC_LIB_NAME} PRIVATE FCGEOMETRYENTITY_BUILDLIB)
add_library(${FC_PROJECT_NAME}::${FC_LIB_NAME} ALIAS ${FC_LIB_NAME})

target_link_libraries(${FC_LIB_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

find_package(${FC_PROJECT_NAME} COMPONENTS
    FCData
    FCGeometryInterface
    FCModelInterface
    REQUIRED
)
target_link_libraries(${FC_LIB_NAME} PUBLIC
    ${FC_PROJECT_NAME}::FCData
    ${FC_PROJECT_NAME}::FCGeometryInterface
    ${FC_PROJECT_NAME}::FCModelInterface
)

fcmacro_set_lib_properties(${FC_LIB_NAME} ${FC_LIB_VERSION})
fcmacro_lib_install()

install(FILES
    ${FC_LIB_HEADER_FILES}
    DESTINATION include/${FC_PROJECT_NAME}/${FC_LIB_NAME}
    COMPONENT headers
)
