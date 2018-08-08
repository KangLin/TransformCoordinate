cmake_minimum_required(VERSION 2.8)

project(GpxModel)

IF(MSVC)
        # This option is to enable the /MP switch for Visual Studio 2005 and above compilers
        OPTION(WIN32_USE_MP "Set to ON to build OpenSceneGraph with the /MP option (Visual Studio 2005 and above)." ON)
        MARK_AS_ADVANCED(WIN32_USE_MP)
        IF(WIN32_USE_MP)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        ENDIF(WIN32_USE_MP)
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

set(OTHER_FILES GpxModel.pc.in)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES} )
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
# Install pc files
configure_file(GpxModel.pc.in ${CMAKE_BINARY_DIR}/GpxModel.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/GpxModel.pc DESTINATION lib/pkgconfig)
