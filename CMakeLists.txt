cmake_minimum_required(VERSION 2.8)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

project(TransformCoordinate)

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
            OUTPUT_VARIABLE TransformCoordinate_VERSION  OUTPUT_STRIP_TRAILING_WHITESPACE
        )
	    
        EXECUTE_PROCESS(
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND ${GIT} rev-parse --short HEAD
            OUTPUT_VARIABLE TransformCoordinate_REVISION OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        
    ENDIF()
ENDIF()
message("TransformCoordinate_VERSION:${TransformCoordinate_VERSION};TransformCoordinate_REVISION:${TransformCoordinate_REVISION}")
if(NOT TransformCoordinate_VERSION)
    SET(TransformCoordinate_VERSION "v0.0.9")
endif()

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

option(BUILD_SHARED_LIBS "Build shared library" ON)
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

add_subdirectory(Src)

# Build options
option(BUILD_APP "Set to ON to build applaction" ON)
if(BUILD_APP)
    add_subdirectory(App)
endif(BUILD_APP)

include(Package/CMakeCPack.cmake)

message("Build app:${BUILD_APP}")
message("Build shared library:${BUILD_SHARED_LIBS}")
message("Build GPXMODEL:${BUILD_GPXMODEL}")
