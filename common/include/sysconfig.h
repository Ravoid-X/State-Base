#pragma once
#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"
using namespace std;
struct SysConfig {
    // Thread Parameters
    bool is_use_camera;
    bool is_use_audio;
    // Camera Parameters
    int camera_id;
    int camera_width;
    int camera_height;
    // Audio Parameters
    string audio_path;
    // Model Parameters
    bool is_load;
    int version;
    string model_path;
    int device;
    double threshold;
    // SQL Parameters
    bool is_use_sql;
    string db_host;
    string db_user;
    string db_pwd;
    string db_name;
    unsigned int db_port;
    // Log Parameters
    bool is_save_log;
    string log_path;
};

inline bool loadConfiguration(const string& config_path, SysConfig& sys_config) {
    YAML::Node config;
    try{
        config = YAML::LoadFile(config_path);
    }catch (const YAML::BadFile& e){
        cerr<<"[YAML] Error: Could not load config file '"<<config_path<<"'."<< endl;
        return false;
    }catch (const YAML::ParserException& e){
        cerr<<"[YAML] Error: Failed to parse '"<<config_path<<"'."<< endl;
        return false;
    }
    try{
        // Thread Parameters
        sys_config.is_use_camera = config["Thread"]["camera"].as<bool>();
        sys_config.is_use_audio = config["Thread"]["audio"].as<bool>();
        // Camera Parameters
        if(sys_config.is_use_camera){
            sys_config.camera_id = config["Camera"]["device_id"].as<int>();
            sys_config.camera_width = config["Camera"]["width"].as<int>();
            sys_config.camera_height = config["Camera"]["height"].as<int>();
        }
        // Audio Parameters
        if(sys_config.is_use_audio){
            sys_config.audio_path = config["Audio"]["path"].as<string>();
        }
        // Model Parameters
        sys_config.is_load = config["Model"]["is_load"].as<bool>();
        if(sys_config.is_load){
            sys_config.version = config["Model"]["version"].as<int>();
            switch(sys_config.version){
                case 0 :
                sys_config.model_path = config["Model"]["path_train"].as<string>();
                break;
                case 1 :
                sys_config.model_path = config["Model"]["path_final"].as<string>();
                break;
            }
            sys_config.device = config["Model"]["device"].as<int>();
            sys_config.threshold = config["Model"]["threshold"].as<double>();
        }
        // SQL Parameters
        sys_config.is_use_sql = config["SQL"]["is_use"].as<bool>();
        if(sys_config.is_use_sql){
            sys_config.db_host = config["SQL"]["host"].as<string>();
            sys_config.db_user = config["SQL"]["user"].as<string>();
            sys_config.db_pwd = config["SQL"]["pwd"].as<string>();
            sys_config.db_name = config["SQL"]["name"].as<string>();
            sys_config.db_port = config["SQL"]["port"].as<unsigned int>();
        }
        // Log Parameters
        sys_config.is_save_log = config["Log"]["is_save"].as<bool>();
        if(sys_config.is_save_log){
            sys_config.log_path = config["Log"]["path"].as<string>();
        }
    }catch (const YAML::Exception& e) {
        cerr<<"[YAML] Error: Failed to read required config value."<<endl;
        return false;
    }
    return true;
}