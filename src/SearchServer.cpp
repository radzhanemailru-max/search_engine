#include "SearchServer.h"
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx) {}

// УБИРАЕМ const из параметра, чтобы совпадало с .h файлом
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, int limit) {
    std::vector<std::vector<RelativeIndex>> result;

    for (const auto& query : queries_input) {
        std::stringstream ss(query);
        std::string word;

        // 1. Собираем уникальные слова из запроса
        std::set<std::string> unique_words;
        while (ss >> word) {
            unique_words.insert(word);
        }

        // 2. Находим все документы, где встречается хотя бы одно слово,
        // и считаем абсолютную релевантность (сумму count)
        std::map<size_t, float> absolute_relevance;
        for (const auto& w : unique_words) {
            std::vector<Entry> word_entries = _index.GetWordCount(w);
            for (const auto& entry : word_entries) {
                absolute_relevance[entry.doc_id] += static_cast<float>(entry.count);
            }
        }

        // 3. Если документов нет, результат пустой
        if (absolute_relevance.empty()) {
            result.emplace_back();
            continue;
        }

        // 4. Находим максимальную абсолютную релевантность
        float max_relevance = 0.0f;
        for (const auto& pair : absolute_relevance) {
            if (pair.second > max_relevance) {
                max_relevance = pair.second;
            }
        }

        // 5. Считаем относительную релевантность (rank)
        std::vector<RelativeIndex> relevance_vector;
        for (const auto& pair : absolute_relevance) {
            float rank = (max_relevance > 0.0f) ? (pair.second / max_relevance) : 0.0f;
            relevance_vector.push_back({pair.first, rank});
        }

        // 6. Сортируем: сначала по убыванию rank, потом по возрастанию doc_id
        std::sort(relevance_vector.begin(), relevance_vector.end(),
                  [](const RelativeIndex& a, const RelativeIndex& b) {
                      if (a.rank != b.rank) {
                          return a.rank > b.rank;
                      }
                      return a.doc_id < b.doc_id;
                  });

        // 7. Обрезаем до лимита (max_responses)
        if (relevance_vector.size() > static_cast<size_t>(limit)) {
            relevance_vector.resize(limit);
        }

        result.push_back(relevance_vector);
    }

    return result;
}
