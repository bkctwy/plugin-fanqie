#ifndef DB_H
#define DB_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fmt/format.h>
#include <SQLiteCpp/SQLiteCpp.h>

using namespace std;

class DB
{
    // SQLite::Database m_db;

private:
    SQLite::Database m_db;

public:
    DB(const string &db_file) : m_db(db_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {}

    bool execute(const string &sql)
    {
        try
        {
            m_db.exec(sql); // 注意这里使用的是成员变量 m_db
            return true;
        }
        catch (const std::exception &e)
        {
            // 处理异常
            fmt::print("SQL execute error: {}\n", e.what());
            return false;
        }
    }

    bool isTableExists(const string &table_name)
    {
        string sql = fmt::format("SELECT name FROM sqlite_master WHERE type='table' AND name='{}'", table_name);
        SQLite::Statement query(m_db, sql);
        return query.executeStep();
    }

    bool isTableEmpty(const string &table_name)
    {
        if (!isTableExists(table_name))
        {
            createTable(table_name);
            return true;
        }
        string sql = fmt::format("SELECT COUNT(*) FROM {}", table_name);
        SQLite::Statement query(m_db, sql);
        query.executeStep();
        int count = query.getColumn(0);
        return count == 0;
    }

    bool createTable(const string &table_name)
    {
        string sql = fmt::format("CREATE TABLE IF NOT EXISTS {} (id STRING PRIMARY KEY, title STRING, url STRING, md5_id STRING,fetch_url STRING, update_time STRING) ", table_name);
        return execute(sql);
    }

    bool insertData(const string &table_name, unordered_map<string, string> data)
    {
        string sql = fmt::format("INSERT INTO {} (id, title, url, md5_id, fetch_url, update_time) SELECT '{}', '{}', '{}', '{}', '{}', '{}' WHERE NOT EXISTS (SELECT 1 FROM {} WHERE id='{}')",
                                 table_name, data["id"], data["title"], data["url"], data["md5_id"], data["fetch_url"], data["update_time"], table_name, data["id"]);
        return execute(sql);
    }

    string getLastUpdateTime(const string &table_name)
    {
        string sql = fmt::format("SELECT * FROM {} ORDER BY update_time DESC LIMIT 1", table_name);
        SQLite::Statement query(m_db, sql);
        if (query.executeStep())
        {
            return query.getColumn(5).getString();
        }
        return "";
    }

    vector<unordered_map<string, string>> readData(const string &table_name)
    {
        // 获取表的列信息
        string pragma_sql = fmt::format("PRAGMA table_info({})", table_name);
        SQLite::Statement pragma_query(m_db, pragma_sql);
        unordered_map<string, int> column_map;
        while (pragma_query.executeStep())
        {
            column_map[pragma_query.getColumn(1)] = pragma_query.getColumn(0); // 列名 -> 列索引
        }

        string sql = fmt::format("SELECT * FROM {}", table_name);
        SQLite::Statement query(m_db, sql);
        vector<unordered_map<string, string>> data;
        while (query.executeStep())
        {
            unordered_map<string, string> row;
            for (const auto &pair : column_map)
            {
                row[pair.first] = query.getColumn(pair.second).getString();
            }
            data.push_back(row);
        }
        return data;
    }
};

#endif // DB_H
