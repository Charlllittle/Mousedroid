cmake_minimum_required(VERSION 3.26)
include(CMakePrintHelpers)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# If find_package doesn't find wxWidgets, try settings the LIB_DIR or (and) the ROOT_DIR manually
if(WIN32)
#     set(wxWidgets_LIB_DIR $ENV{WXWIN}/lib/gcc1320_x64_dll)
#     set(wxWidgets_ROOT_DIR $ENV{WXWIN})
endif()

project(Mousedroid)

find_package(wxWidgets REQUIRED net gl core base adv)


file(GLOB_RECURSE SRC_FILES src/*.cpp)

if(WIN32)

    enable_language(RC)
    set(CMAKE_CXX_FLAGS "-Wl,-subsystem,windows")
    set(SRC_FILES ${SRC_FILES} src/input/WindowsInputManager.cpp resource.rc)
    set(ASIO_PATH $ENV{ASIO})
    set(LIBS ws2_32 wsock32)

else()

    set(SRC_FILES ${SRC_FILES} src/input/LinuxInputManager.cpp)
    
    # Set this if you downloaded the asio library
    # set(ASIO_PATH <path_to_your_asio>)

endif()

add_executable(Mousedroid ${SRC_FILES})

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(Mousedroid ${wxWidgets_LIBRARIES})
endif()

target_link_libraries(Mousedroid ${LIBS})

target_include_directories(Mousedroid PRIVATE ./include ${ASIO_PATH}/include)

if(WIN32)
    # $<$<CONFIG:Debug>:d> expands to 'd' in Debug builds, empty in Release,
    # selecting the correct wxWidgets DLL variant automatically.
    add_custom_command(TARGET Mousedroid POST_BUILD
        # wxWidgets DLLs
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{WXWIN}/lib/gcc1320_x64_dll/wxbase32u$<$<CONFIG:Debug>:d>_gcc1320_x64.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{WXWIN}/lib/gcc1320_x64_dll/wxbase32u$<$<CONFIG:Debug>:d>_net_gcc1320_x64.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{WXWIN}/lib/gcc1320_x64_dll/wxmsw32u$<$<CONFIG:Debug>:d>_core_gcc1320_x64.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{WXWIN}/lib/gcc1320_x64_dll/wxmsw32u$<$<CONFIG:Debug>:d>_gl_gcc1320_x64.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$ENV{WXWIN}/lib/gcc1320_x64_dll/wxmsw32u$<$<CONFIG:Debug>:d>_adv_gcc1320_x64.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        # ADB
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/adb/adb.exe"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/adb/AdbWinApi.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/adb/AdbWinUsbApi.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        # MinGW runtime DLLs
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/dll/libgcc_s_seh-1.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/dll/libstdc++-6.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/dll/libwinpthread-1.dll"
            $<TARGET_FILE_DIR:Mousedroid>
        # Icon
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/app.ico"
            $<TARGET_FILE_DIR:Mousedroid>
        # config.ini — only created if it doesn't already exist
        COMMAND ${CMAKE_COMMAND}
            -DDEST_DIR=$<TARGET_FILE_DIR:Mousedroid>
            -P "${CMAKE_SOURCE_DIR}/cmake/create_config.cmake"
        COMMENT "Copying runtime dependencies..."
    )
endif()
