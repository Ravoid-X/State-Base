#pragma once
#include <string>
#include <memory>
#include <vector>
#include <shared_mutex>
using namespace std;
// 向前声明 MySQL C API 的结构体
struct st_mysql;
typedef struct st_mysql MYSQL;
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