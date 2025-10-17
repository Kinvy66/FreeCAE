cmake_minimum_required(VERSION 3.5)

project(APP VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SARibbonBar_DIR ${FREECAE_LIB_DIR}/SARibbonBar/lib/cmake/SARibbonBar)
set(QTADS_DIR ${FREECAE_LIB_DIR}/QtADS/lib/cmake/qtadvanceddocking-qt6)
set(VTK_DIR ${FREECAE_LIB_DIR}/vtk/lib/cmake/vtk-9.4)
set(SPDLOG_DIR ${FREECAE_LIB_DIR}/spdlog/lib/cmake/spdlog)
set(OpenCASCADE_DIR ${FREECAE_LIB_DIR}/occt/cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(SARibbonBar REQUIRED)
find_package(qtadvanceddocking-qt6 REQUIRED PATHS ${QTADS_DIR})
find_package(VTK REQUIRED)
find_package(spdlog REQUIRED)
# find_package(OpenCASCADE REQUIRED)


set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(APP
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        AppWidget.h AppWidget.cpp AppWidget.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET APP APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(APP SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(APP
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# file(GLOB LIBS
#     ${FREECAE_LIB_DIR}/occt/win64/vc14/bind/*.dll
#     ${FREECAE_LIB_DIR}/occt/win64/vc14/bind/*.dll
# )

target_link_libraries(APP PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(APP PUBLIC SARibbonBar::SARibbonBar)
target_link_libraries(APP PUBLIC ads::qtadvanceddocking-qt6)
target_link_libraries(APP PRIVATE ${VTK_LIBRARIES})
target_link_libraries(APP PRIVATE spdlog::spdlog)
target_link_libraries(APP PRIVATE  ${OpenCASCADE_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.APP)
endif()
set_target_properties(APP PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS APP
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(APP)
endif()
