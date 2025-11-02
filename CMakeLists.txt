cmake_minimum_required(VERSION 3.15)
set(PROJ_NAME State_Base)
project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(USE_OPENCV "Enable OpenCV features" ON)
option(USE_LIBTORCH "Enable LibTorch features" OFF)

if(USE_OPENCV)
    message(STATUS "OpenCV is ENABLED")
    if(NOT DEFINED OpenCV_DIR) 
        if(WIN32)
            set(OpenCV_DIR "C:/ruanjian/opencv/build")
        elseif(UNIX AND NOT APPLE)
            set(OpenCV_DIR "/usr/lib/x86_64-linux-gnu/cmake/opencv4")
        endif()
    endif()
    find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
    # 添加 C++ 宏: #ifdef WITH_OPENCV
    add_compile_definitions(WITH_OPENCV)
else()
    message(STATUS "OpenCV is DISABLED")
endif()

if(USE_LIBTORCH)
    message(STATUS "LibTorch is ENABLED")
    if(NOT DEFINED Torch_DIR)
         if(WIN32)
            set(Torch_DIR "C:/ruanjian/libtorch/share/cmake/Torch")
         elseif(UNIX AND NOT APPLE)
            set(Torch_DIR "/home/xcc/libtorch28cpu/share/cmake/Torch")
         endif()
    endif()
    find_package(Torch REQUIRED)
    add_compile_definitions(WITH_LIBTORCH)
    # 确保在 Linux 上使用正确的 ABI
    if(UNIX AND NOT APPLE)
        add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)
    endif()
else()
    message(STATUS "LibTorch is DISABLED")
endif()

add_library(yaml_cpp INTERFACE)
target_include_directories(yaml_cpp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(yaml_cpp INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libyaml-cpp.so
)

add_subdirectory(common)
add_subdirectory(state)
add_subdirectory(thread)
add_executable(${PROJ_NAME} main.cpp)
target_link_libraries(${PROJ_NAME}
    PRIVATE
    common_lib
    state_lib
    thread_lib
    yaml_cpp
)