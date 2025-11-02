add_library(state_lib)

file(GLOB STATE_SOURCES "src/*.cpp" "src/*.c")
target_sources(state_lib PRIVATE ${STATE_SOURCES})
target_include_directories(state_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(state_lib
    PRIVATE
    common_lib
)