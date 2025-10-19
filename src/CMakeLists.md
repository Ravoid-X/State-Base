# --- 将模块编译成库 ---
add_library(utils_lib utils/Logger.cpp utils/ConfigParser.cpp)
target_include_directories(utils_lib PUBLIC utils)

add_library(models_lib) # 这是一个只有头文件的接口库
target_include_directories(models_lib PUBLIC models)

add_library(capture_lib capture/CameraSource.cpp)
target_include_directories(capture_lib PUBLIC capture)
target_link_libraries(capture_lib PUBLIC models_lib utils_lib)

add_library(processing_lib processing/ImagePreprocessor.cpp)
target_include_directories(processing_lib PUBLIC processing)
target_link_libraries(processing_lib PUBLIC models_lib)

add_library(core_lib core/FigureClassifier.cpp core/PointDetector.cpp)
target_include_directories(core_lib PUBLIC core)
target_link_libraries(core_lib PUBLIC models_lib utils_lib)

add_library(visualizer_lib visualizer/CvVisualizer.cpp)
target_include_directories(visualizer_lib PUBLIC visualizer)
target_link_libraries(visualizer_lib PUBLIC models_lib)

add_library(app_lib app/GestureApp.cpp)
target_include_directories(app_lib PUBLIC app)
target_link_libraries(app_lib PUBLIC capture_lib processing_lib core_lib visualizer_lib utils_lib models_lib)

# --- 创建 recognizer 可执行文件 ---
add_executable(recognizer main.cpp)

# --- 链接所有库 ---
target_link_libraries(recognizer PRIVATE
    app_lib
    ${OpenCV_LIBS}
    ${TORCH_LIBRARIES}
)

# --- 设置包含目录 ---
target_include_directories(recognizer PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
)

# --- 构建后事件 ---
# 将运行时需要的文件夹复制到 recognizer.exe 旁边
add_custom_command(TARGET recognizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:recognizer>/config
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/model $<TARGET_FILE_DIR:recognizer>/model
    COMMENT "Copying runtime data for recognizer..."
)