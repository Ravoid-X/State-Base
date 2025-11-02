# 查找平台原生的线程库 (pthread on Linux, Windows threads on Windows)
find_package(Threads REQUIRED)

add_library(thread_lib)

file(GLOB THREAD_SOURCES "src/*.cpp" "src/*.c")
target_sources(thread_lib PRIVATE ${THREAD_SOURCES})
target_include_directories(thread_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(thread_lib
    PUBLIC
    Threads::Threads
    PRIVATE
    common_lib
)