cmake_minimum_required(VERSION 2.8)

project(GpxModel)

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/MP") 
ENDIF(MSVC)

set(HEAD_FILES
    gpx_model.h
    )
set(SOURCE_FILES
    actfile.cpp
    gpx_model.cpp
    gpxfile.cpp
    nmeafile.cpp
    utils.c
    uxmlpars.c)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
#set_target_properties(${PROJECT_NAME}
#    PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
#    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib")
    
# Install head files
INSTALL(FILES ${HEAD_FILES} DESTINATION "include")
