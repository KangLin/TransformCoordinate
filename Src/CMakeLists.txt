project(TransformCoordinate)

option(BUILD_GPXMODEL "Set to ON to build GpxModel" ON)
if(BUILD_GPXMODEL)
    add_subdirectory(GpxModel)
    SET(GpxModel_DEFINITIONS ${GpxModel_DEFINITIONS} -DBUILD_GPXMODEL)
    add_definitions(${GpxModel_DEFINITIONS})
    set(GPXMODEL_LIB GpxModel)
endif()

set(INSTALLHEADER_FILES
    TransformCoordinate.h
    )
set(HEAD_FILES
    ${INSTALLHEADER_FILES}
    )
set(SOURCE_FILES
    TransformCoordinate.cpp)

#设置资源文件
SET(RCC_FILES
    ${TRANSLATIONS_RESOURCE_FILES}
    )

add_library(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEAD_FILES}
    ${RCC_FILES})
if(BUILD_GPXMODEL)
    target_link_libraries(${PROJECT_NAME} PUBLIC GpxModel)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR})
GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
set(INSTALLHEADER_FILES ${INSTALLHEADER_FILES} 
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})
# Install head files
set_property(
    TARGET
        ${PROJECT_NAME}
    PROPERTY
        PUBLIC_HEADER "${INSTALLHEADER_FILES}"
)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION})

if(ANDROID)
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "libs/${ANDROID_ABI}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TransformCoordinate
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
else()
    # Install target
    INSTALL(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Config
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TransformCoordinate
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
