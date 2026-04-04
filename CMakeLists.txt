# 作者：康林<kl222@126.com>

cmake_minimum_required(VERSION 3.18)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(TransformCoordinate)

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

SET(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

SET(TransformCoordinate_VERSION "v1.1.2" CACHE STRING "Project version")
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
if(NOT(TransformCoordinate_VERSION MATCHES "[0-9]+\\.[0-9]+\\.[0-9]+[+\\-^_~0-9A-Za-z]*"))
    SET(TransformCoordinate_VERSION "1.1.2")
endif()
string(REPLACE "v" "" TransformCoordinate_VERSION ${TransformCoordinate_VERSION})
if(TransformCoordinate_REVISION)
    string(REPLACE "v" "" TransformCoordinate_REVISION ${TransformCoordinate_REVISION})
endif()
message("TransformCoordinate_VERSION: ${TransformCoordinate_VERSION}; TransformCoordinate_REVISION: ${TransformCoordinate_REVISION}")

add_subdirectory(Src)

# Build options
option(BUILD_APP "Set to ON to build applaction" ON)
if(BUILD_APP)
    add_subdirectory(App)
endif(BUILD_APP)

include(Package/CMakeCPack.cmake)

message("Build app:${BUILD_APP}")
message("Build shared library:${BUILD_SHARED_LIBS}")
message("Build GPXMODEL:${WITH_GPXMODEL}")
