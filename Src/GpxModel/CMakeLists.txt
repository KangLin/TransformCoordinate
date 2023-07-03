cmake_minimum_required(VERSION 2.8)

project(GpxModel)

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

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    $<$<CXX_COMPILER_ID:GNU,Clang>:$<IF:$<CONFIG:Debug>, -g -ggdb, -O3>>
    )
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:_DEBUG DEBUG>
    )
if(NOT MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fPIC>
    )
endif()
if(LIBKML_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GPX_LIBS})
    foreach(d ${GpxModel_DEFINITION})
        target_compile_definitions(${PROJECT_NAME} PUBLIC ${d})
    endforeach()
    target_include_directories(${PROJECT_NAME} PRIVATE "${LIBKML_INCLUDE_DIRS}")
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
list(APPEND HEAD_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${HEAD_FILES}" # Install head files
    VERSION ${TransformCoordinate_VERSION})
if(NOT ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endif()

# Install target
SET(INSTALL_LIBRARY "${CMAKE_INSTALL_LIBDIR}")
if(WIN32)
	SET(INSTALL_LIBRARY "${CMAKE_INSTALL_BINDIR}")
endif()
INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Config
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Runtime
    LIBRARY DESTINATION "${INSTALL_LIBRARY}"
        COMPONENT Runtime
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        COMPONENT Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GpxModel
        COMPONENT Development
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

export(TARGETS ${PROJECT_NAME}
       APPEND FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
       )
# Install cmake configure files
install(EXPORT ${PROJECT_NAME}Config
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
            COMPONENT Development
        )
write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${TransformCoordinate_VERSION}
    COMPATIBILITY AnyNewerVersion)
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake"
        COMPONENT Development)
