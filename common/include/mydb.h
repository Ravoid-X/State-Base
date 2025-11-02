#pragma once
#include <string>
#include <memory>
#include <vector>
#include <shared_mutex>
#include <mysql/mysql.h> 
using namespace std;

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool connect(const string& host, const string& user, 
                 const string& password, const string& dbname, 
                 unsigned int port);
    void disconnect();
    bool createTable(const string& query);
    bool insert(const string& query);
    bool update(const string& query);
    bool select(const string& query, vector<vector<string>>& results);
private:
    MYSQL* m_connection;
    mutable shared_mutex m_rw_mutex;
    bool write_internal(const string& query);
};