# FCData 库 - 替代 FITKCore 的基类（不依赖 FITK）
cmake_minimum_required(VERSION 3.12)

fcmacro_lib_setting(
    "FCData"
    "FCData | https://github.com/kinvy66"
    0
    0
    1
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} ${FC_MIN_QT_VERSION} COMPONENTS Core REQUIRED)

file(GLOB FC_LIB_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB FC_LIB_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${FC_LIB_NAME} SHARED
    ${FC_LIB_HEADER_FILES}
    ${FC_LIB_SOURCE_FILES}
)
target_compile_definitions(${FC_LIB_NAME} PRIVATE FCDATA_BUILD)
add_library(${FC_PROJECT_NAME}::${FC_LIB_NAME} ALIAS ${FC_LIB_NAME})

target_include_directories(${FC_LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/${FC_PROJECT_NAME}/${FC_LIB_NAME}>
)
target_link_libraries(${FC_LIB_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    ${FC_PROJECT_NAME}::FCUtils
)

fcmacro_set_lib_properties(${FC_LIB_NAME} ${FC_LIB_VERSION})
fcmacro_lib_install()

install(FILES
    ${FC_LIB_HEADER_FILES}
    DESTINATION include/${FC_PROJECT_NAME}/${FC_LIB_NAME}
    COMPONENT headers
)
