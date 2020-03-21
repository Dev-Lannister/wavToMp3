cmake_minimum_required(VERSION 3.4.1)

#设置变量SRC_DIR为lamemp3的所在路径
set(SRC_DIR lamemp3)

#指定头文件所在，可以多次调用，指定多个路径
include_directories(lamemp3)

aux_source_directory(lamemp3 SRC_LIST)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI})

#将前面目录下所有的文件都添加进去
add_library( # Sets the name of the library.
        lamemp3

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        GetVersion.cpp
        Convert.cpp
        ${SRC_LIST} )


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )

target_link_libraries( # Specifies the target library.
        lamemp3

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        log)