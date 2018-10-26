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

SET(BUILD_VERSION "v0.0.4")
# Find Git Version Patch
IF(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    if(NOT GIT)
        SET(GIT $ENV{GIT})
    endif()
    if(NOT GIT)
        FIND_PROGRAM(GIT NAMES git git.exe git.cmd)
    endif()
    IF(GIT)
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} describe --tags
            OUTPUT_VARIABLE GIT_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
	    if(NOT GIT_VERSION)
            EXECUTE_PROCESS(
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND ${GIT} rev-parse --short HEAD
                OUTPUT_VARIABLE GIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE
            )
        endif()
        SET(BUILD_VERSION ${GIT_VERSION})
    ENDIF()
ENDIF()
message("BUILD_VERSION:${BUILD_VERSION}")

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
    SET(GpxModel_DEFINITIONS ${GpxModel_DEFINITIONS} -DBUILD_GPXMODEL)
    add_definitions(${GpxModel_DEFINITIONS})
    set(GPXMODEL_LIB GpxModel)
endif()
    
set(OTHER_FILES appveyor.yml .travis.yml)
set(HEAD_FILES
        TransformCoordinate.h)
set(SOURCE_FILES
        TransformCoordinate.cpp)
    
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_NAME} ${GPXMODEL_LIB})
    
if (BUILD_SHARED_LIBS)
	add_definitions(-DBUILD_SHARED_LIBS)
    if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
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
configure_file(TransformCoordinateConfig.cmake.in ${CMAKE_BINARY_DIR}/TransformCoordinateConfig.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/TransformCoordinateConfig.cmake DESTINATION .)
if(MINGW)
    # Install pc files
    configure_file(TransformCoordinate.pc.in ${CMAKE_BINARY_DIR}/TransformCoordinate.pc @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/TransformCoordinate.pc DESTINATION lib/pkgconfig)
endif()
    
# Build options
option(BUILD_APP "Set to ON to build applaction" ON)

#需要的QT组件
SET(QT_COMPONENTS Core Gui Widgets)
find_package(Qt5 COMPONENTS ${QT_COMPONENTS})
if(Qt5_FOUND)
    SET(BUILD_APP ON)
endif()

if(BUILD_APP)
    #打开 qt 编译工具
    SET(CMAKE_AUTOUIC ON)
    SET(CMAKE_AUTOMOC ON)
    SET(CMAKE_AUTORCC ON)
    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    SET(CMAKE_VERBOSE_MAKEFILE ON)

    FOREACH(_COMPONENT ${QT_COMPONENTS})
        SET(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5${_COMPONENT}_LIBRARIES})
    ENDFOREACH()
        
    SET(QT_INSTALL_DIR "${Qt5_DIR}/../../..")
        
    set(HEAD_FILES
            mainwindow.h
            Widgets/DlgAbout/DlgAbout.h
            )
    SET(SOURCES_FILES 
            mainwindow.cpp
            main.cpp
            Widgets/DlgAbout/DlgAbout.cpp
            )
    SET(SOURCE_UI_FILES 
            mainwindow.ui
            Widgets/DlgAbout/DlgAbout.ui 
            )
    #设置资源文件
    SET(RCC_FILES
            Resource/Resource.qrc
            Resource/sink/dark/style.qrc
            ${CMAKE_CURRENT_BINARY_DIR}/translations.qrc
            )
   
    #翻译
    include(cmake/Qt5CorePatches.cmake)    
    include(cmake/Translations.cmake)
    
    add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")
    
    add_executable(${PROJECT_NAME}App ${SOURCES_FILES} ${HEAD_FILES} ${SOURCE_UI_FILES} AppIcon.rc ${RCC_FILES})
    SET(APP_LIBS ${PROJECT_NAME} ${QT_LIBRARIES})
    if(BUILD_GPXMODEL)
        add_definitions(${GpxModel_DEFINITIONS})
        SET(APP_LIBS ${APP_LIBS} ${GPXMODEL_LIB})
    endif()
    target_link_libraries(${PROJECT_NAME}App ${APP_LIBS})
    target_include_directories(${PROJECT_NAME}App PUBLIC "${PROJECT_BINARY_DIR}")
        
    include(GNUInstallDirs OPTIONAL)
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}App
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            )
 
    IF(WIN32)
        IF(CMAKE_BUILD_TYPE MATCHES Release AND MINGW)
            #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
            add_custom_command(TARGET ${PROJECT_NAME}App POST_BUILD
                COMMAND strip "${PROJECT_BINARY_DIR}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}"
                )
        ENDIF()

        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中  
        SET(APP_PATH "${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}")
        add_custom_command(TARGET ${PROJECT_NAME}App POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/${PROJECT_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy "${APP_PATH}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/."
            COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
            --compiler-runtime
            --verbose 7
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}App${CMAKE_EXECUTABLE_SUFFIX}"
            )
        INSTALL(DIRECTORY "${PROJECT_BINARY_DIR}/${PROJECT_NAME}/." DESTINATION "bin")
        
    ENDIF(WIN32)
    
    IF(ANDROID)
        GENERATED_DEPLOYMENT_SETTINGS()
        find_program(ANT NAMES ant) # PATHS  "/usr/bin")
        MESSAGE("ant:${ANT}\n")
    
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/libRabbitIm.so ${PROJECT_BINARY_DIR}/android-build/libs/${ANDROID_ABI}/libRabbitIm.so
            #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
            COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
                --input ${PROJECT_BINARY_DIR}/android-libRabbitIm.so-deployment-settings.json
                --output ${PROJECT_BINARY_DIR}/android-build
                --verbose
                #--ant /usr/bin/ant
                #--jdk $ENV{JAVA_HOME}
        )
    ENDIF(ANDROID)
endif(BUILD_APP)

message("Build app:${BUILD_APP}")
message("Build shared library:${BUILD_SHARED_LIBS}")
message("Build GPXMODEL:${BUILD_GPXMODEL}")
