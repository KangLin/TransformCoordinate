cmake_minimum_required(VERSION 2.8)

project(TransformCoordinate)

if(POLICY CMP0083)
    cmake_policy(SET CMP0083 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

# 建立要删除的文件
CONFIGURE_FILE(
    "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
# 建立删除目标
ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")

SET(BUILD_VERSION "v0.0.9")
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
set(VERSION ${BUILD_VERSION})

if("Debug" STREQUAL CMAKE_BUILD_TYPE)
    add_definitions(-D_DEBUG)
endif()

IF(MSVC)
    # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
    OPTION(WIN32_USE_MP "Set to ON to build with the /MP option (Visual Studio 2005 and above)." ON)
    MARK_AS_ADVANCED(WIN32_USE_MP)
    IF(WIN32_USE_MP)
        #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        add_compile_options(/MP)
    ENDIF(WIN32_USE_MP)
    add_compile_options(/utf-8)
ENDIF(MSVC)
IF(MINGW)
    add_compile_options("-std=c++0x")
ENDIF()
SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")
if (BUILD_SHARED_LIBS)
	add_definitions(-DBUILD_SHARED_LIBS)
    if (CMAKE_COMPILER_IS_GNUCXX AND NOT MINGW)
       # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
       # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
       # does.
       add_definitions("-fPIC")
    endif()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(GenerateExportHeader)
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)

CHECK_INCLUDE_FILE_CXX("string" HAVE_STRING_H)
check_include_file("math.h" HAVE_MATH_H)

check_function_exists("fabs" HAVE_FABS)
check_function_exists("sin" HAVE_SIN)
check_function_exists("cos" HAVE_COS)

option(BUILD_SHARED_LIBS "Build shared library" ON)

add_subdirectory(Src)

# Build options
option(BUILD_APP "Set to ON to build applaction" ON)
if(BUILD_APP)
    if( "${RabbitCommon_DIR}" STREQUAL "" )
        set(RabbitCommon_DIR $ENV{RabbitCommon_DIR})
        if( "${RabbitCommon_DIR}" STREQUAL "" )
            set(RabbitCommon_DIR ${CMAKE_SOURCE_DIR}/../RabbitCommon)
        endif()
    endif()
    
    if(DEFINED RabbitCommon_DIR AND EXISTS ${RabbitCommon_DIR}/Src)
        add_subdirectory(${RabbitCommon_DIR}/Src ${CMAKE_BINARY_DIR}/RabbitCommon)
    else()
        message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon")
        message("   ag:")
        message("       git clone https://github.com/KangLin/RabbitCommon.git")
        message("2. Then set cmake value or environment variable RabbitCommon_DIR to download root dirctory.")
        message("   ag:")
        message(FATAL_ERROR "       cmake -DRabbitCommon_DIR= ")
    endif()
    add_subdirectory(App)
    
    add_custom_target(install-runtime
      COMMAND
         "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=Runtime 
         -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
    )
endif(BUILD_APP)

message("Build app:${BUILD_APP}")
message("Build shared library:${BUILD_SHARED_LIBS}")
message("Build GPXMODEL:${BUILD_GPXMODEL}")
