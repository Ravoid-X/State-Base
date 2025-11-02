add_library(state_lib)

file(GLOB STATE_SOURCES "src/*.cpp" "src/*.c")
target_sources(state_lib PRIVATE ${STATE_SOURCES})
target_include_directories(state_lib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(state_lib
    PRIVATE
    common_lib
)

if(USE_OPENCV)
    target_link_libraries(state_lib
        PUBLIC
        OpenCV::OpenCV
    )
endif()
if(USE_LIBTORCH)
    target_link_libraries(state_lib
        PUBLIC
        Torch::Torch
    )
endif()