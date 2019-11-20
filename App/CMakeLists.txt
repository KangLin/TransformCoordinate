project(TransformCoordinateApp)

#需要的QT组件
SET(QT_COMPONENTS Core Gui Widgets)
if(ANDROID)
    SET(QT_COMPONENTS ${QT_COMPONENTS} AndroidExtras)
endif()
find_package(Qt5 COMPONENTS ${QT_COMPONENTS})
message("QT_VERSION:${Qt5_VERSION}")
if(Qt5_VERSION VERSION_LESS 5.10.0 AND ANDROID)
    message(FATAL_ERROR "Qt must great 5.10.0")
endif()
FOREACH(_COMPONENT ${QT_COMPONENTS})
    SET(QT_LIBRARIES ${QT_LIBRARIES} Qt5::${_COMPONENT})
ENDFOREACH()
get_filename_component(QT_INSTALL_DIR "${Qt5_DIR}/../../.." ABSOLUTE)
message("QT_INSTALL_DIR:${QT_INSTALL_DIR}")

#打开 qt 编译工具
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

set(HEAD_FILES
    mainwindow.h)
set(SOURCES_FILES
    main.cpp 
    mainwindow.cpp)
set(SOURCE_UI_FILES
    mainwindow.ui)

#翻译
SET(TRANSLATIONS_NAME ${PROJECT_NAME})
include(${CMAKE_SOURCE_DIR}/cmake/Translations.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)

SET(RCC_FILES
    ${TRANSLATIONS_RESOURCE_FILES}
    Resource/Resource.qrc
    )
if(WIN32)
    SET(RCC_FILES
        ${RCC_FILES}
        Resource/AppIcon.rc)
endif()
add_definitions(-DBUILD_VERSION="${BUILD_VERSION}")
add_definitions(-DBUILD_ARCH="${BUILD_ARCH}")
add_definitions(-DBUILD_PLATFORM="${BUILD_PLATFORM}")

if(ANDROID)
    add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEAD_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES_FILES} ${HEAD_FILES} ${SOURCE_UI_FILES} ${RCC_FILES})
endif()
add_dependencies(${TRANSLATIONS_NAME} translations_${TRANSLATIONS_NAME})
SET(APP_LIBS ${QT_LIBRARIES} TransformCoordinate)
if(TARGET RabbitCommon)
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DRABBITCOMMON)
    target_include_directories(${PROJECT_NAME}
        PRIVATE "${RabbitCommon_DIR}/Src" "${RabbitCommon_DIR}/Src/export")
    set(APP_LIBS ${APP_LIBS} RabbitCommon)
endif()
target_link_libraries(${PROJECT_NAME} ${APP_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Src" "${CMAKE_BINARY_DIR}")
if(${CMAKE_CXX_COMPILER} MATCHES ".*/(g\\+\\+)(\\..*)")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "-mwindows")
elseif(${CMAKE_CXX_COMPILER} MATCHES ".*/(cl)(\\..*)")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE
        "/SUBSYSTEM:WINDOWS\",5.01\" /ENTRY:mainCRTStartup")
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION})
if(UNIX AND NOT ANDROID)
    set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${PROJECT_NAME})
endif()

# Install target
if(ANDROID)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        LIBRARY DESTINATION "libs/${ANDROID_ABI}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
elseif(WIN32)
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
else()
    INSTALL(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            COMPONENT Runtime
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            COMPONENT Runtime
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )
endif()

if(UNIX)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/debian/TransformCoordinate.desktop
        DESTINATION "share/applications"
        COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/Resource/png/TransformCoordinate.png
        DESTINATION "share/pixmaps"
        COMPONENT Runtime)
endif()
IF(WIN32)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/App/Resource/png/TransformCoordinate.ico
        DESTINATION .
        COMPONENT Runtime)
    INSTALL(FILES ${CMAKE_SOURCE_DIR}/Install/Install.nsi
        DESTINATION "${CMAKE_BINARY_DIR}"
        COMPONENT Runtime)
ENDIF()

# Install other files
set(OTHER_FILES ${CMAKE_SOURCE_DIR}/License.md
    ${CMAKE_SOURCE_DIR}/Authors.md
    ${CMAKE_SOURCE_DIR}/ChangeLog.md
    ${CMAKE_SOURCE_DIR}/Authors_zh_CN.md
    ${CMAKE_SOURCE_DIR}/ChangeLog_zh_CN.md)
if(ANDROID)
    INSTALL(FILES ${OTHER_FILES} DESTINATION "assets" COMPONENT Runtime)
else()
    INSTALL(FILES ${OTHER_FILES} DESTINATION "." COMPONENT Runtime)
endif()

IF("Release" STREQUAL CMAKE_BUILD_TYPE AND WIN32)
    SET(RUNTIME_FILE "${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
    IF(CMAKE_BUILD_TYPE MATCHES Release AND MINGW)
        #windeployqt 分发时，是根据是否 strip 来判断是否是 DEBUG 版本,而用mingw编译时,qt没有自动 strip
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND strip "$<TARGET_FILE:${PROJECT_NAME}>"
            )
    ENDIF()

    #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中  
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${QT_INSTALL_DIR}/bin/windeployqt"
        --compiler-runtime
        --verbose 7
        "$<TARGET_FILE:${PROJECT_NAME}>"
        )
    INSTALL(DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>/"
        DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT Runtime)
ENDIF()

IF(ANDROID)
    Set(JSON_FILE ${CMAKE_BINARY_DIR}/android_deployment_settings.json)
    GENERATED_DEPLOYMENT_SETTINGS(NAME ${JSON_FILE}
        APPLACTION "${CMAKE_BINARY_DIR}/bin/lib${PROJECT_NAME}.so")

    set(ANDROID_DELOP_PARA --gradle)
    if(Qt5_VERSION VERSION_LESS 5.8.0)
        find_program(ANT NAMES ant) # PATHS  "/usr/bin")
        MESSAGE("ant:${ANT}\n")
        if(ANT_FOUND)
            set(ANDROID_DELOP_PARA --ant ${ANT})
        endif()
    endif()

    add_custom_target(APK #TARGET ${PROJECT_NAME} POST_BUILD
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" ${CMAKE_INSTALL_PREFIX}/libs/${ANDROID_ABI}/$<TARGET_FILE_NAME:${PROJECT_NAME}>
        #COMMAND ${CMAKE_COMMAND} -E make_directory $${CMAKE_INSTALL_PREFIX}/libs/${ANDROID_ABI}
        #COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:${PROJECT_NAME}>/*" ${CMAKE_INSTALL_PREFIX}/libs/${ANDROID_ABI}
        #注意 需要把 ${QT_INSTALL_DIR}/bin 加到环境变量PATH中
        COMMAND "${QT_INSTALL_DIR}/bin/androiddeployqt"
            --output ${CMAKE_INSTALL_PREFIX}
            --input ${JSON_FILE}
            --verbose
            --android-platform ${ANDROID_PLATFORM}
            ${ANDROID_DELOP_PARA}
            #--jdk $ENV{JAVA_HOME}
    )
ENDIF(ANDROID)
