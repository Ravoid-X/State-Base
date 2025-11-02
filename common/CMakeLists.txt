add_library(common_lib)
# --- src ---
file(GLOB COMMON_SOURCES "src/*.cpp" "src/*.c")
target_sources(common_lib PRIVATE ${COMMON_SOURCES})
# --- YAML ---
add_library(yaml_cpp INTERFACE)
target_include_directories(yaml_cpp INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(yaml_cpp INTERFACE ${PROJECT_SOURCE_DIR}/lib/libyaml-cpp.so.0.8.0)
# --- MySQL ---
if(USE_MYSQL)
    message(STATUS "MYSQL is ENABLED")
    # 1. 找到已经安装的 mysql_config 程序
    find_program(MYSQL_CONFIG_EXECUTABLE mysql_config)
    if(NOT MYSQL_CONFIG_EXECUTABLE)
        message(FATAL_ERROR "mysql_config not found. Cannot configure MySQL.")
    endif()
    # 2. 运行 "mysql_config --variable=pkgincludedir" 来获取头文件目录
    execute_process(
        COMMAND ${MYSQL_CONFIG_EXECUTABLE} --variable=pkgincludedir
        OUTPUT_VARIABLE MySQL_INCLUDE_DIRS
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # 3. 运行 "mysql_config --libs" 来获取所有链接器标志
    execute_process(
        COMMAND ${MYSQL_CONFIG_EXECUTABLE} --libs
        OUTPUT_VARIABLE MySQL_LIBRARIES
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    message(STATUS "MYSQL is DISABLED")
endif()
# --- OpenCV ---
if(USE_OPENCV)
    message(STATUS "COMMON: OpenCV is ENABLED")
    if(WIN32)
        set(OpenCV_DIR "C:/ruanjian/opencv/build")
    elseif(UNIX AND NOT APPLE)
        set(OpenCV_DIR "/usr/local/lib/cmake/opencv4")
    endif()
    find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)
    add_compile_definitions(WITH_OPENCV)
endif()
# --- LibTorch ---
if(USE_LIBTORCH)
    message(STATUS "COMMON: LibTorch is ENABLED")
    if(WIN32)
        set(Torch_DIR "C:/ruanjian/libtorch/share/cmake/Torch")
    elseif(UNIX AND NOT APPLE)
        set(Torch_DIR "/home/xcc/libtorch28cpu/share/cmake/Torch")
    endif()
    find_package(Torch REQUIRED)
    add_compile_definitions(WITH_LIBTORCH)
    if(UNIX AND NOT APPLE)
        add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)
    endif()
endif()
# --- include ---
target_include_directories(common_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
if(USE_MYSQL)
    target_include_directories(common_lib PUBLIC ${MySQL_INCLUDE_DIRS})
endif()
# --- lib ---
target_link_libraries(common_lib
    PUBLIC
    yaml_cpp
)
if(USE_OPENCV)
    target_link_libraries(common_lib PUBLIC ${OpenCV_LIBRARIES})
endif()
if(USE_LIBTORCH)
    target_link_libraries(common_lib PUBLIC ${Torch_LIBRARIES})
endif()
if(USE_MYSQL)
    target_link_libraries(common_lib PUBLIC ${MySQL_LIBRARIES})
endif()