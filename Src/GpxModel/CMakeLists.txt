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
    ${SOURCE_FILES}
    actfile.cpp
    gpx_model.cpp
    gpxfile.cpp
    nmeafile.cpp
    utils.c
    uxmlpars.c)

find_package(LibKML)
if(LIBKML_FOUND)
    SET(GpxModel_DEFINITION -DBUILD_LIBKML ${LIBKML_DEFINITIONS})
    SET(GPX_LIBS ${LIBKML_LIBRARIES})
    set(SOURCE_FILES ${SOURCE_FILES} LibKML.cpp)
endif()

option(BUILD_SHARED_LIBS "Build shared library" ON)
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
if(LIBKML_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${GPX_LIBS})
    target_compile_definitions(${PROJECT_NAME} PUBLIC ${GpxModel_DEFINITION})
    target_include_directories(${PROJECT_NAME} PRIVATE "LIBKML_INCLUDE_DIRS")
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION})

GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
set(HEAD_FILES ${HEAD_FILES} 
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})

# Install head files
set_property(
    TARGET
        ${PROJECT_NAME}
    PROPERTY
        PUBLIC_HEADER "${HEAD_FILES}"
)

if(ANDROID)
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "libs/${ANDROID_ABI}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GpxModel
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
else()
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GpxModel
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()
export(TARGETS ${PROJECT_NAME}
       APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
       )
# Install cmake configure files
install(EXPORT ${PROJECT_NAME}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
        )
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${BUILD_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake")
