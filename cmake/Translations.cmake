SET(TS_FILES
    Resource/translations/ar.ts
    Resource/translations/be.ts
    Resource/translations/bg.ts
    Resource/translations/cs.ts
    Resource/translations/da.ts
    Resource/translations/de.ts
    Resource/translations/el.ts
    Resource/translations/eo.ts
    Resource/translations/es.ts
    Resource/translations/et.ts
    Resource/translations/fa.ts
    Resource/translations/fi.ts
    Resource/translations/fr.ts
    Resource/translations/he.ts
    Resource/translations/hr.ts
    Resource/translations/hu.ts
    Resource/translations/it.ts
    Resource/translations/ja.ts
    Resource/translations/jbo.ts
    Resource/translations/ko.ts
    Resource/translations/lt.ts
    Resource/translations/mk.ts
    Resource/translations/nl.ts
    Resource/translations/no_nb.ts
    Resource/translations/pl.ts
    Resource/translations/pr.ts
    Resource/translations/pt.ts
    Resource/translations/ro.ts
    Resource/translations/ru.ts
    Resource/translations/sk.ts
    Resource/translations/sl.ts
    Resource/translations/sr.ts
    Resource/translations/sr_Latn.ts
    Resource/translations/sv.ts
    Resource/translations/sw.ts
    Resource/translations/ta.ts
    Resource/translations/tr.ts
    Resource/translations/ug.ts
    Resource/translations/uk.ts
    Resource/translations/zh_CN.ts
    Resource/translations/zh_TW.ts)

OPTION(OPTION_RABBITIM_TRANSLATIONS "Refresh translations on compile" ON)
MESSAGE("Refresh translations on compile: ${OPTION_RABBITIM_TRANSLATIONS}\n")
IF(OPTION_RABBITIM_TRANSLATIONS)
    FIND_PACKAGE(Qt5 CONFIG REQUIRED LinguistTools) #语言工具
    IF(NOT Qt5_LRELEASE_EXECUTABLE)
        MESSAGE(WARNING "Could not find lrelease. Your build won't contain translations.")
    ELSE(NOT Qt5_LRELEASE_EXECUTABLE)
        qt5_add_translation(QM_FILES ${TS_FILES}) #生成翻译资源文件
        set(RCC_OPTIONS -compress 9 -threshold 0)
        qt5_add_resources(
          RCC_FILES
          Resource/translations/translations.qrc
          DEPENDS ${QM_FILES}
          OPTIONS ${RCC_OPTIONS}
        )
        
        ADD_CUSTOM_TARGET(translations ALL DEPENDS ${QM_FILES})
        add_dependencies(${PROJECT_NAME} translations)
        #调试时使用，复制到编译目录
        foreach(_file ${QM_FILES})
            IF(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        #COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} ${CMAKE_CURRENT_BINARY_DIR}/android-build/translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${QT_INSTALL_DIR}/translations/qt_*.qm ${CMAKE_CURRENT_BINARY_DIR}/android-build/translations
                        )
            ELSE(ANDROID)
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E make_directory translations
                        COMMAND ${CMAKE_COMMAND} -E copy ${_file} translations
                        )
            ENDIF(ANDROID)
        endforeach()
        #IF(EXISTS "${QT_INSTALL_DIR}/translations/qt_*.qm" AND NOT ANDROID)
        #    FILE(COPY ${QT_INSTALL_DIR}/translations/qt_*.qm DESTINATION "translations")
        #ENDIF()

        #安装1:翻译
        INSTALL(DIRECTORY "${QT_INSTALL_DIR}/translations"
                DESTINATION "."
                FILES_MATCHING PATTERN "qt_*.qm")
        #INSTALL(FILES "${QM_FILES}" DESTINATION "translations" CONFIGURATIONS Release)
        INSTALL(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/translations"
                DESTINATION "."
                )
    ENDIF(NOT Qt5_LRELEASE_EXECUTABLE)
ENDIF(OPTION_RABBITIM_TRANSLATIONS)
