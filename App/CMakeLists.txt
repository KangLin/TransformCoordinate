project(TransformCoordinateApp)

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

if(NOT RabbitCommon_DIR)
    set(RabbitCommon_DIR $ENV{RabbitCommon_DIR})
    if(NOT RabbitCommon_DIR)
        set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../RabbitCommon)
    endif()
endif()
if(RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
    message("Use RabbitCommon source code")
    add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
else()
    find_package(RabbitCommon)
    if(NOT RabbitCommon_FOUND)
        message("RabbitCommon_DIR is not found. Please use one of the following ways to set it:")
        message("1. Set RabbitCommon_DIR to the install prefix of RabbitCommon.")
        message("2. Set RabbitCommon_DIR to source code root of RabbitCommon.")
        message("2.1 Please download the source code of RabbitCommon from https://github.com/KangLin/RabbitCommon")
        message("   ag:")
        message("       git clone https://github.com/KangLin/RabbitCommon.git")
        message("2.2 Then set cmake variable or environment variable RabbitCommon_DIR to download root directory.")
        message("   ag:")
        message("       cmake -DRabbitCommon_DIR= ")
        message(FATAL_ERROR "RabbitCommon_DIR isn't set.")
    endif()
endif()

#需要的QT组件
if(NOT DEFINED QT_VERSION_MAJOR)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core)
endif()
SET(QT_COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt${QT_VERSION_MAJOR}_VERSION}")
if(Qt${QT_VERSION_MAJOR}_VERSION VERSION_LESS 6.0.0 AND ANDROID)
    message(FATAL_ERROR "Qt must great 6.0.0")
endif()
if(Qt${QT_VERSION_MAJOR}_FOUND)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        LIST(APPEND QT_LIBRARIES Qt${QT_VERSION_MAJOR}::${_COMPONENT})
    ENDFOREACH()
endif()
get_filename_component(QT_INSTALL_DIR "${Qt${QT_VERSION_MAJOR}_DIR}/../../.." ABSOLUTE)

set(HEAD_FILES
    mainwindow.h)
set(SOURCES_FILES
    main.cpp 
    mainwindow.cpp)
set(SOURCE_UI_FILES
    mainwindow.ui)

SET(RCC_FILES
    ${TRANSLATIONS_RESOURCE_FILES}
    Resource/Resource.qrc
    )
if(WIN32)
    SET(RCC_FILES
        ${RCC_FILES}
        Resource/AppIcon.rc)
endif()

ADD_TARGET(NAME ${PROJECT_NAME}
    ISEXE
    ISWINDOWS
    VERSION ${TransformCoordinate_VERSION}
    SOURCE_FILES ${SOURCES_FILES} ${HEAD_FILES} ${SOURCE_UI_FILES} ${RCC_FILES}
    PRIVATE_LIBS ${QT_LIBRARIES} TransformCoordinate RabbitCommon
    PRIVATE_DEFINITIONS RABBITCOMMON
        TransformCoordinate_REVISION="${TransformCoordinate_REVISION}"
        TransformCoordinate_VERSION="${TransformCoordinate_VERSION}"
    )

if(UNIX)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/share/TransformCoordinate.desktop
        DESTINATION "share/applications"
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/share/TransformCoordinate.sh
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/Resource/icons/black/svg/TransformCoordinate.svg
        DESTINATION "share/pixmaps"
            COMPONENT Runtime)
endif()
IF(WIN32)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/Resource/png/TransformCoordinate.ico
        DESTINATION .
            COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Install/Install.nsi
        DESTINATION "${CMAKE_BINARY_DIR}"
            COMPONENT Runtime)
ENDIF()

# Install other files
set(OTHER_FILES ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    ${CMAKE_SOURCE_DIR}/Authors_zh_CN.md
    ${CMAKE_SOURCE_DIR}/ChangeLog_zh_CN.md)
INSTALL_FILE(SOURCES ${OTHER_FILES} DESTINATION "." COMPONENT Runtime)

INSTALL_ICON_THEME(SOURCES ${CMAKE_SOURCE_DIR}/App/Resource/icons)
