add_library(algorithm_lib)

file(GLOB ALGORITHM_SOURCES "src/*.cpp" "src/*.c")
target_sources(algorithm_lib PRIVATE ${ALGORITHM_SOURCES})
target_include_directories(algorithm_lib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(algorithm_lib
    PRIVATE
    common_lib
)

if(USE_OPENCV)
    target_link_libraries(algorithm_lib
        PUBLIC
        OpenCV::OpenCV
    )
endif()
if(USE_LIBTORCH)
    target_link_libraries(algorithm_lib
        PUBLIC
        Torch::Torch
    )
endif()