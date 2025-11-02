cmake_minimum_required(VERSION 3.15)
set(PROJ_NAME State_Base)
project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(USE_OPENCV "Enable OpenCV features" ON)
option(USE_LIBTORCH "Enable LibTorch features" OFF)
option(USE_MYSQL "Enable MYSQL features" ON)

add_subdirectory(common)
add_subdirectory(state)
add_subdirectory(thread)
add_executable(${PROJ_NAME} main.cpp)
target_link_libraries(${PROJ_NAME}
    PRIVATE
    common_lib
    state_lib
    thread_lib
)