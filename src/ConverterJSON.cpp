
#include "ConverterJSON.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream config("config.json");
    if(!config.is_open())
        throw std::runtime_error("config file is missing");

    json j;
    config >> j;

    std::vector<std::string> docs;

    for(auto &file : j["files"]) {
        std::ifstream f(file);
        if(!f.is_open()){
            std::cerr << "File missing: " << file << std::endl;
            continue;
        }

        std::string text((std::istreambuf_iterator<char>(f)),
                          std::istreambuf_iterator<char>());

        docs.push_back(text);
    }

    return docs;
}

int ConverterJSON::GetResponsesLimit() {
    std::ifstream config("config.json");
    json j;
    config >> j;

    if(j["config"].contains("max_responses"))
        return j["config"]["max_responses"];

    return 5;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream req("requests.json");

    json j;
    req >> j;

    std::vector<std::string> result;

    for(auto &r : j["requests"])
        result.push_back(r);

    return result;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int,float>>> answers) {

    json result;
    int id = 1;

    for(auto &req : answers) {

        std::string key = "request";
        key += (id < 10 ? "00" : (id < 100 ? "0" : ""));
        key += std::to_string(id);

        if(req.empty()) {
            result["answers"][key]["result"] = "false";
        } else {
            result["answers"][key]["result"] = "true";

            for(auto &p : req) {
                json item;
                item["docid"] = p.first;
                item["rank"] = p.second;
                result["answers"][key]["relevance"].push_back(item);
            }
        }

        id++;
    }

    std::ofstream out("answers.json");
    out << result.dump(4);
}
