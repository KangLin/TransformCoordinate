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

option(BUILD_SHARED_LIBS "Build shared library" OFF)

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEAD_FILES} )
#set_target_properties(${PROJECT_NAME}
#    PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
#    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
#    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

if (BUILD_SHARED_LIBS)
      if (CMAKE_COMPILER_IS_GNUCXX)
         # Just setting CMAKE_POSITION_INDEPENDENT_CODE should be enough to set
         # -fPIC for GCC but sometimes it still doesn't get set, so make sure it
         # does.
         add_definitions("-fPIC")
      endif()
      set(CMAKE_POSITION_INDEPENDENT_CODE ON)
      
      # 添加静态库，关键词为static，
      # ADD_LIBRARY (hello STATIC ${LIBHELLO_SRC})
      # 仍然用hello作为target名时，是不能成功创建所需的静态库的，
      # 因为hello作为一个target是不能重名的， 故把上面的hello修改为hello_static
      # 同理，你不需要写全libhello_static.a 
      # 只需要填写hello即可，cmake系统会自动为你生成 libhello_static.X
      add_library(${PROJECT_NAME}_static STATIC ${SOURCE_FILES} ${HEAD_FILES})
      # 按照一般的习惯，静态库名字跟动态库名字应该是一致的，只是扩展名不同；
      # 即：静态库名为 libhello.a； 动态库名为libhello.so ；
      # 所以，希望 "hello_static" 在输出时，不是"hello_static"，而是以"hello"的名字显示，故设置如下：
      SET_TARGET_PROPERTIES (${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    
      # cmake在构建一个新的target时，会尝试清理掉其他使用这个名字的库，
      # 因此，在构建libhello.a时，就会清理掉libhello.so. 
      # 为了回避这个问题，比如再次使用SET_TARGET_PROPERTIES定义 CLEAN_DIRECT_OUTPUT属性。
      SET_TARGET_PROPERTIES (${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
      SET_TARGET_PROPERTIES (${PROJECT_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
      
      # 按照规则，动态库是应该包含一个版本号的，
      # VERSION指代动态库版本，SOVERSION指代API版本。
      #SET_TARGET_PROPERTIES (${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION} SOVERSION 1)
      SET_TARGET_PROPERTIES (${PROJECT_NAME} PROPERTIES VERSION ${BUILD_VERSION})
      
      # Install target
      INSTALL(TARGETS ${PROJECT_NAME}_static
              RUNTIME DESTINATION "bin"
              LIBRARY DESTINATION "lib"
              ARCHIVE DESTINATION "lib")
endif()

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
