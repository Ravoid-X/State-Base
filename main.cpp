#include "utils/Logger.h"
#include "utils/ConfigParser.h"
#include "app/GestureApp.h"
#include "capture/CameraSource.h"
#include "processing/ImagePreprocessor.h"
#include "core/FigureClassifier.h"
#include "core/PointDetector.h"
#include "visualizer/CvVisualizer.h"

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;

// 解析配置文件中的逗号分隔字符串
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    Logger::getInstance().log("--- Gesture Recognizer Starting ---");
    ConfigParser config;
    if (!config.load("config/settings.ini")) {
        Logger::getInstance().error("Failed to load config/settings.ini");
        return -1;
    }

    try {
        // --- 模块实例化 ---
        auto dataSource = make_unique<CameraSource>(
            config.getInt("Camera", "id", 0),
            config.getInt("Camera", "width", 640),
            config.getInt("Camera", "height", 480)
        );

        auto preprocessor = make_unique<ImagePreprocessor>(
            config.getInt("Processing", "hsv_lower_h", 0),
            config.getInt("Processing", "hsv_lower_s", 48),
            config.getInt("Processing", "hsv_lower_v", 80),
            config.getInt("Processing", "hsv_upper_h", 20),
            config.getInt("Processing", "hsv_upper_s", 255),
            config.getInt("Processing", "hsv_upper_v", 255)
        );

        unique_ptr<IVisualizer> visualizer = nullptr;
        if (config.getBool("Display", "show_window", true)) {
            visualizer = make_unique<CvVisualizer>("Gesture Recognition");
        }

        // --- 根据配置创建不同的识别器 (策略模式) ---
        unique_ptr<IRecognizer> recognizer = nullptr;
        string recognizerType = config.getString("Recognizer", "recognizer_type", "classification");

        if (recognizerType == "classification") {
            Logger::getInstance().log("Using Figure Classification model.");
            auto class_names_str = config.getString("Recognizer", "class_names", "");
            auto class_names_vec = split(class_names_str, ',');
            if (class_names_vec.empty() || class_names_str.empty()) {
                throw runtime_error("`class_names` is empty in settings.ini for classification mode.");
            }

            recognizer = make_unique<FigureClassifier>(
                config.getString("Recognizer", "classification_model_path", "model/figure.pt"),
                config.getInt("Recognizer", "model_input_width", 224),
                config.getInt("Recognizer", "model_input_height", 224),
                class_names_vec
            );
        } else if (recognizerType == "keypoint") {
            Logger::getInstance().log("Using Hand Keypoint Detection model.");
            recognizer = make_unique<PointDetector>(
                config.getString("Recognizer", "keypoint_model_path", "model/point.pt"),
                config.getInt("Recognizer", "model_input_width", 224),
                config.getInt("Recognizer", "model_input_height", 224)
            );
        } else {
            throw runtime_error("Invalid recognizer_type in settings.ini: " + recognizerType);
        }

        // --- 组装并运行App ---
        GestureApp app(
            move(dataSource),
            move(preprocessor),
            move(recognizer),
            move(visualizer)
        );
        app.run();

    } catch (const exception& e) {
        Logger::getInstance().error("An unhandled exception occurred: " + string(e.what()));
        return -1;
    }

    Logger::getInstance().log("--- Gesture Recognizer Shutdown ---");
    return 0;
}