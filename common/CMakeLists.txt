add_library(common_lib)

file(GLOB COMMON_SOURCES "src/*.cpp" "src/*.c")
target_sources(common_lib PRIVATE ${COMMON_SOURCES})
target_include_directories(common_lib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
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