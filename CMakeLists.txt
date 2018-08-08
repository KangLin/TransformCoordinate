cmake_minimum_required(VERSION 2.8)

project(TransformCoordinate)

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

# 设置安装前缀
IF(NOT CMAKE_INSTALL_PREFIX) 
    SET(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/${PROJECT_NAME} CACHE STRING "Install path prefix.") 
ENDIF() 

SET(BUILD_VERSION 1.0.0)
# Find Git Version Patch
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    FIND_PROGRAM(GIT git)
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} rev-parse --short HEAD
            OUTPUT_VARIABLE BUILD_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        MESSAGE("Git Version: ${BUILD_VERSION}\n")
    ENDIF(GIT)
ENDIF()

IF(MSVC)
        # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
        OPTION(WIN32_USE_MP "Set to ON to build OpenSceneGraph with the /MP option (Visual Studio 2005 and above)." ON)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)
ENDIF(MSVC)

option(BUILD_GPXMODEL  "Set to ON to build GpxModel"  ON)
if(BUILD_GPXMODEL)
    add_subdirectory(GpxModel)
    add_definitions(-DBUILD_GPXMODEL)
    set(GPXMODEL_LIB GpxModel)
endif()

set(HEAD_FILES
    TransformCoordinate.h)
set(SOURCE_FILES
    TransformCoordinate.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
#add_dependencies(${PROJECT_NAME} GpxModel)

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib")
# Install head files
INSTALL(FILES ${HEAD_FILES} DESTINATION "include")
# Install cmake files
configure_file(TransformCoordinate.cmake.in ${CMAKE_BINARY_DIR}/TransformCoordinateConfig.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/TransformCoordinateConfig.cmake DESTINATION .)

# Build options
option(BUILD_APP  "Set to ON to build applaction"  ON)

if(BUILD_APP)
    #打开 qt 编译工具
    SET(CMAKE_AUTOUIC ON)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_AUTORCC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(CMAKE_VERBOSE_MAKEFILE ON)
    
    #判断 qt 版本
    SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")
    #需要的QT组件
    SET(QT_COMPONENTS Core Gui Widgets)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        FIND_PACKAGE(Qt5${_COMPONENT} REQUIRED)
        SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
    ENDFOREACH()
   
    set(HEAD_FILES
        mainwindow.h)
    SET(SOURCES_FILES 
        mainwindow.cpp
        main.cpp)
    SET(SOURCE_UI_FILES 
        mainwindow.ui)
    add_executable(${PROJECT_NAME}_app ${SOURCES_FILES} ${HEAD_FILES} ${SOURCE_UI_FILES})
    target_link_libraries(${PROJECT_NAME}_app ${PROJECT_NAME} ${GPXMODEL_LIB} ${QT_LIBRARIES})
endif()
