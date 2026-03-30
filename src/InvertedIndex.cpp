
#include "InvertedIndex.h"
#include <sstream>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {

    docs = input_docs;
    freq_dictionary.clear();

    for(size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {

        std::stringstream ss(docs[doc_id]);
        std::string word;

        std::map<std::string,int> counter;

        while(ss >> word)
            counter[word]++;

        for(auto &p : counter) {
            freq_dictionary[p.first].push_back({doc_id,(size_t)p.second});
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {

    if(freq_dictionary.count(word))
        return freq_dictionary[word];

    return {};
}
