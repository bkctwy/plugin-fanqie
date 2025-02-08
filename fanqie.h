// Fanqie.h
#ifndef FANQIE_H
#define FANQIE_H

#include "IPlugin.h"
#include "utils/utils.h"
// #include "utils/db.h"
#include <string>
#include <regex>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using namespace std;
using json = nlohmann::json;

class Fanqie : public IPlugin
{
public:
    string getName() const override
    {
        return this->plugin_name;
    }

    string getSiteId() const override
    {
        return this->site_id;
    }

    string getPluginId() const override
    {
        return this->plugin_id;
    }

    void init(string id) override
    {
        this->id = id;
    }

    string getAuthor() override
    {
        cpr::Response response = cpr::Get(cpr::Url{detail_api_url + this->id});
        json jsonResponse = json::parse(response.text);
        this->author = jsonResponse["data"][0]["author"].get<string>();
        return this->author;
    }

    void getCover() override
    {
        if (this->title.empty())
        {
            getTitle();
        }
        cpr::Response response = cpr::Get(cpr::Url{detail_api_url + this->id});
        json jsonResponse = json::parse(response.text);
        string cover_url = jsonResponse["data"][0]["expand_thumb_url"].get<string>();
        this->saveCover(cover_url);
    }

    string getTitle() override
    {
        cpr::Response response = cpr::Get(cpr::Url{detail_api_url + this->id});
        json jsonResponse = json::parse(response.text);
        this->title = jsonResponse["data"][0]["book_name"].get<string>();
        return this->title;
    }

protected:
    string plugin_id = "fanqie";
    string plugin_name = "Fanqie";
    string site_name = "Fanqie";
    string site_id = "fanqie";
    string site_url = "https://fanqienovel.com";

    string detail_api_url = "https://fanqienovel.com/reading/bookapi/multi-detail/v/?aid=1967&iid=1&version_code=999&book_id=";
    string catalog_api_url = "https://fanqienovel.com/api/reader/directory/detail?bookId=";
    string chapter_api_url = "https://novel.snssdk.com/api/novel/reader/full/v1/?item_id=";

    vector<unordered_map<string, string>>
    getCatalog() override
    {
        if (this->title.empty())
        {
            getTitle();
        }
        if (db.isTableEmpty(this->title) == true)
        {
            db.createTable(this->title);
            cpr::Response response = cpr::Get(cpr::Url{this->catalog_api_url + this->id});
            json jsonResponse = json::parse(response.text);
            // fmt::print("{}\n", jsonResponse.dump(4));
            for (const auto &volumes : jsonResponse["data"]["chapterListWithVolume"])
            {
                for (const auto &chapter : volumes)
                {
                    string title = chapter["title"];
                    string url = fmt::format("https://fanqienovel.com/reader/{}", chapter["itemId"].get<string>());
                    // string url = "https://fanqienovel.com/reader/" + to_string(chapter["itemId"]);
                    string id = chapter["itemId"].get<string>();
                    string md5_id = utils::stringToMD5(id);
                    string fetch_url = (this->chapter_api_url + id);
                    unordered_map<string, string> catalog = utils::initCatalogMap(title, url, id, md5_id, fetch_url);
                    db.insertData(this->title, catalog);
                    this->catalog_data.push_back(catalog);
                }
            }
        }
        else
        {
            this->catalog_data = db.readData(this->title);
        }
        return this->catalog_data;
    }

    void parseChapter(unordered_map<string, string> chapter_data) override
    {
        if (this->author.empty())
        {
            getAuthor();
        }

        int index = stoi(chapter_data["index"]);
        json chapter_json = json::parse(chapter_data["content"]);
        string main_chapter = chapter_json["data"]["content"];
        regex html_tag_regex("(<.*?>)+");
        main_chapter = regex_replace(main_chapter, html_tag_regex, "\n");
        string chapter_head = fmt::format("{}\n{}\n---\n", chapter_data["title"], this->author);
        string full_chapter = chapter_head + main_chapter;

        saveChapter(chapter_data["title"], full_chapter);
    }

    string getCatalogPage() override
    {
        return "";
    }

    // private:
    //     DB db = DB(this->db_path);
};
#endif // FANQIE_H