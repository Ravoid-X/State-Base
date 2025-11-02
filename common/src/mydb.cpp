#include <mysql/mysql.h> 
#include <iostream>
#include "common/include/mydb.h"
using namespace std;

DatabaseManager::DatabaseManager() : m_connection(nullptr) {
    mysql_library_init(0, nullptr, nullptr);
}
DatabaseManager::~DatabaseManager() {
    disconnect();
    mysql_library_end();
}

bool DatabaseManager::connect(const string& host, const string& user, 
                            const string& password, const string& dbname, 
                            unsigned int port){
    // connect() 是一种“写入”操作 (修改 m_connection)
    lock_guard<shared_mutex> lock(m_rw_mutex);
    if (m_connection) {
        mysql_close(m_connection);
    }
    m_connection = mysql_init(nullptr);
    if (!m_connection) {
        cerr<<"[DBManager] Error: mysql_init() failed."<<endl;
        return false;
    }
    if (!mysql_real_connect(m_connection, host.c_str(), user.c_str(), 
                            password.c_str(), dbname.c_str(), port, 
                            nullptr, 0)) {
        cerr << "[DBManager] Error: mysql_real_connect() failed: " 
                  << mysql_error(m_connection) << endl;
        mysql_close(m_connection);
        m_connection = nullptr;
        return false;
    }
    cout<<"[DBManager]: connection successful to '"<<dbname<<"'."<<endl;
    return true;
}

void DatabaseManager::disconnect() {
    // disconnect() 也是一种“写入”操作
    lock_guard<shared_mutex> lock(m_rw_mutex);
    if (m_connection) {
        mysql_close(m_connection);
        m_connection = nullptr;
        cout << "[DBManager]: connection closed." << endl;
    }
}
bool DatabaseManager::write_internal(const string& query) {
    lock_guard<shared_mutex> lock(m_rw_mutex);
    if (!m_connection) {
        cerr<<"[DBManager] Error: Write cannot execute, not connected."<<endl;
        return false;
    }
    if (mysql_query(m_connection, query.c_str())) {
        cerr << "[DBManager] Error: write mysql_query() failed for query [" 
                  << query << "]: " << mysql_error(m_connection) << endl;
        return false;
    }
    return true;
}
bool DatabaseManager::createTable(const string& query) {
    cout << "[DBManager] Executing CREATE TABLE..." << endl;
    return write_internal(query);
}
bool DatabaseManager::insert(const string& query) {
    return write_internal(query);
}
bool DatabaseManager::update(const string& query) {
    return write_internal(query);
}

bool DatabaseManager::select(const string& query, vector<vector<string>>& results) {
    shared_lock<shared_mutex> lock(m_rw_mutex);
    if (!m_connection) {
        cerr << "[DBManager] Error: Read cannot execute, not connected." << endl;
        return false;
    }
    // 执行查询
    if (mysql_query(m_connection, query.c_str())) {
        cerr << "[DBManager] Error: Read mysql_query() failed: " 
                  << mysql_error(m_connection) << endl;
        return false;
    }
    // 存储结果
    MYSQL_RES* result = mysql_store_result(m_connection);
    if (!result) {
        if(mysql_field_count(m_connection) > 0) {
             cerr << "[DBManager] Error: Read mysql_store_result() failed: "
                       << mysql_error(m_connection) << endl;
             return false;
        }
        return true; 
    }
    // 转换结果
    results.clear();
    MYSQL_ROW row;
    unsigned int num_fields = mysql_num_fields(result);
    while ((row = mysql_fetch_row(result))) {
        vector<string> row_data;
        row_data.reserve(num_fields);
        for(unsigned int i = 0; i < num_fields; i++) {
            row_data.push_back(row[i] ? row[i] : "NULL");
        }
        results.push_back(row_data);
    }
    // 释放内存
    mysql_free_result(result);
    return true;
}