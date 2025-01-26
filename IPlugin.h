// IPlugin.h
#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <string>
#include <fmt/os.h>
#include <cpr/cpr.h>
#include <fmt/format.h>
#include <future>
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "utils/db.h"
#include "utils/task.h"
#include <thread>
#include <chrono>
// #include "utils/progress_bar.h"
// #include "logger.h"

using namespace std;
namespace fs = filesystem;
using json = nlohmann::json;

class IPlugin
{
public:
    IPlugin()
    {
        this->readConfig();
        this->db = DB(this->db_path);
    }

    virtual ~IPlugin() = default;
    virtual string getName() const = 0;
    virtual string getSiteId() const = 0;
    virtual string getPluginId() const = 0;
    virtual void init(string id) = 0;

    virtual vector<unordered_map<string, string>> getIndex() = 0;
    virtual string getTitle() = 0;
    virtual string getAuthor() = 0;
    virtual void getCover() = 0;

    // void setId(string id)
    // {
    //     this->id = id;
    // }

    virtual void unload() {};

    void fetchAllChapter()
    {
        getIndex();

        fs::path save_path = fmt::format("{}/{}", this->novels_folder, this->title);
        fs::create_directories(save_path);

        TaskManager manager(stoi(this->max_workers), stoi(this->sleep_time));
        auto callback = [this](const unordered_map<string, string> &chapter_data)
        {
            parseChapter(chapter_data);
        };
        for (int i = 0; i < index_data.size(); i++)
        {
            if (isDownloaded(index_data[i]["title"]))
            {
                continue;
            }
            manager.add_task([this, i]()
                             { return this->fetchOneChapter(i); }, callback);
        }
        manager.wait_all();
    }

protected:
    string plugin_id;
    string plugin_name;

    string id;
    string title;
    string site_name;
    string site_id;
    string site_url;
    string author;
    string index_page_text;
    string index_page_url;
    vector<unordered_map<string, string>> index_data;

    // config.json
    string data_folder;
    string novels_folder;
    string covers_folder;
    string logs_folder;
    string db_path = "cache.db";
    string sleep_time;
    string max_workers;

    DB db = DB(db_path);
    // Logger logger = Logger("log.txt");

    bool isDownloaded(string title)
    {
        string file_path = this->novels_folder + "/" + this->title + "/" + title + ".txt";
        ifstream file(file_path);
        return file.is_open();
    }

    void saveChapter(string title, string content)
    {
        string file_name = this->novels_folder + "/" + this->title + "/" + title + ".txt";
        auto out = fmt::output_file(file_name);
        out.print(content);
    }

    virtual string getIndexPage()
    {
        if (this->index_page_text.empty())
        {
            cpr::Response response = cpr::Get(cpr::Url{this->index_page_url});
            this->index_page_text = response.text;
        }
        return this->index_page_text;
    };

    virtual void parseChapter(unordered_map<string, string> chapter_data) = 0;

    unordered_map<string, string> fetchOneChapter(int index)
    {
        fmt::print("Fetching chapter... ({}/{})\n", index + 1, this->index_data.size());
        cpr::Response response = cpr::Get(cpr::Url{this->index_data[index]["fetch_url"]});
        string content = response.text;
        unordered_map<string, string> chapter_data;
        chapter_data["status"] = "success";
        chapter_data["index"] = to_string(index);
        chapter_data["title"] = this->index_data[index]["title"];
        chapter_data["content"] = content;
        return chapter_data;
    }

    void readConfig()
    {
        ifstream config_file("config.json");
        json data = json::parse(config_file);
        this->data_folder = data["data_folder"].get<string>();
        this->novels_folder = data_folder + "/" + data["novels_folder"].get<string>();
        this->covers_folder = data_folder + "/" + data["covers_folder"].get<string>();
        this->logs_folder = data_folder + "/" + data["logs_folder"].get<string>();
        // this->db_path = data["db_path"].get<string>();
        this->sleep_time = data["sleep_time"].get<string>();
        this->max_workers = data["max_workers"].get<string>();

        fs::path data_path = this->data_folder;
        fs::path novels_path = this->novels_folder;
        fs::path covers_path = this->covers_folder;
        fs::path logs_path = this->logs_folder;

        fs::create_directories(data_path);
        fs::create_directories(novels_path);
        fs::create_directories(covers_path);
        fs::create_directories(logs_path);
    }
};

#endif // IPLUGIN_H