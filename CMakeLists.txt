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

SET(BUILD_VERSION "0.0.1")
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
    
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_GPXMODEL "Set to ON to build GpxModel" ON)
if(BUILD_GPXMODEL)
    add_subdirectory(GpxModel)
    SET(GpxModel_DEFINITIONS -DBUILD_GPXMODEL)
    add_definitions(${GpxModel_DEFINITIONS})
    set(GPXMODEL_LIB GpxModel)
endif()
    
set(OTHER_FILES appveyor.yml .travis.yml)
set(HEAD_FILES
        TransformCoordinate.h)
set(SOURCE_FILES
        TransformCoordinate.cpp)
    
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_NAME} ${GPXMODEL_LIB})
#add_dependencies(${PROJECT_NAME} ${GPXMODEL_LIB})
    
if (BUILD_SHARED_LIBS)
    if (CMAKE_COMPILER_IS_GNUCXX)
       # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
       # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
       # does.
       add_definitions("-fPIC")
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    if(WIN32)
        add_definitions(-DDLL_EXPORT)
    endif(WIN32)
endif()
    
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
if(MINGW)
    # Install pc files
    configure_file(TransformCoordinate.pc.in ${CMAKE_BINARY_DIR}/TransformCoordinate.pc @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/TransformCoordinate.pc DESTINATION lib/pkgconfig)
endif()
    
# Build options
option(BUILD_APP  "Set to ON to build applaction"  ON)
    
if(BUILD_APP)
    #打开 qt 编译工具
    SET(CMAKE_AUTOUIC ON)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_AUTORCC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(CMAKE_VERBOSE_MAKEFILE ON)
        
    #需要的QT组件
    SET(QT_COMPONENTS Core Gui Widgets)
    find_package(Qt5 COMPONENTS ${QT_COMPONENTS} REQUIRED)
    FOREACH(_COMPONENT ${QT_COMPONENTS})
        SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
    ENDFOREACH()
        
    SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")
       
    set(HEAD_FILES
            mainwindow.h)
    SET(SOURCES_FILES 
            mainwindow.cpp
            main.cpp)
    SET(SOURCE_UI_FILES 
            mainwindow.ui)
    add_executable(${PROJECT_NAME}App ${SOURCES_FILES} ${HEAD_FILES} ${SOURCE_UI_FILES})
    target_link_libraries(${PROJECT_NAME}App ${PROJECT_NAME} ${GPXMODEL_LIB} ${QT_LIBRARIES})
        
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}App
            RUNTIME DESTINATION "bin"
            LIBRARY DESTINATION "lib"
            ARCHIVE DESTINATION "lib")
        
    IF(WIN32)
        IF(CMAKE_BUILD_TYPE MATCHES Release AND MINGW)
            #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
            add_custom_command(TARGET ${PROJECT_NAME}App POST_BUILD
                COMMAND strip "${PROJECT_BINARY_DIR}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}"
                )
        ENDIF()
            
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中  
        #add_custom_command(TARGET ${PROJECT_NAME}App POST_BUILD
        #    COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PROJECT_NAME}"
        #    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/."
        #    COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
        #    --compiler-runtime
        #    --verbose 7
        #    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}"
        #    )
        #INSTALL(DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/." DESTINATION "bin")
    ENDIF(WIN32)
endif(BUILD_APP)
