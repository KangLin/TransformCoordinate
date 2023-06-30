project(TransformCoordinate)

option(BUILD_GPXMODEL "Set to ON to build GpxModel" ON)
if(BUILD_GPXMODEL)
    add_subdirectory(GpxModel)
endif()

set(INSTALLHEADER_FILES
    TransformCoordinate.h
    )
set(HEAD_FILES
    ${INSTALLHEADER_FILES}
    )
set(SOURCE_FILES
    TransformCoordinate.cpp)

add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEAD_FILES}
    )
if(TARGET GpxModel)
    target_link_libraries(${PROJECT_NAME} PUBLIC GpxModel)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_GPXMODEL)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
set(INSTALLHEADER_FILES ${INSTALLHEADER_FILES} 
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${INSTALLHEADER_FILES}"  # Install head files
    VERSION ${BUILD_VERSION})
if(NOT ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endif()

# Install target
INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Config
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TransformCoordinate
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

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
