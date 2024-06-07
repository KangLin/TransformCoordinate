# 作者：康林<kl222@126.com>

project(TransformCoordinate)

option(WITH_GPXMODEL "Set to ON to build GpxModel" ON)
if(WITH_GPXMODEL)
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
    target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_GPXMODEL)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)
target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    $<$<CXX_COMPILER_ID:GNU,Clang>:$<IF:$<CONFIG:Debug>, -g -ggdb, -O3>>
    )
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>: _DEBUG DEBUG>)
if(NOT MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-fPIC>
    )
endif()

GENERATE_EXPORT_HEADER(${PROJECT_NAME})
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)
list(APPEND INSTALLHEADER_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h)
file(COPY ${CMAKE_CURRENT_BINARY_DIR}/${LOWER_PROJECT_NAME}_export.h
    DESTINATION ${CMAKE_BINARY_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    PUBLIC_HEADER "${INSTALLHEADER_FILES}"  # Install head files
    INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib;$ORIGIN/../lib/${CMAKE_LIBRARY_ARCHITECTURE}"
    )
if(TransformCoordinate_VERSION)
    string(REPLACE "v" "" TransformCoordinate_VERSION ${TransformCoordinate_VERSION})
    if(NOT PARA_SOVERSION)
        string(FIND ${TransformCoordinate_VERSION} "." _VERSION_MAJOR_POS)
        string(SUBSTRING ${TransformCoordinate_VERSION} 0 ${_VERSION_MAJOR_POS} PARA_SOVERSION)
    endif()
    string(REPLACE "v" "" PARA_SOVERSION ${PARA_SOVERSION})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${TransformCoordinate_VERSION}
    )
endif()
if(PARA_SOVERSION)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SOVERSION ${PARA_SOVERSION}
    )
endif()
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

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
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/TransformCoordinate
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
