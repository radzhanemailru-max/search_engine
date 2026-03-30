#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>

int main() {
    try {
        ConverterJSON converter;

        auto docs = converter.GetTextDocuments();

        InvertedIndex idx;
        idx.UpdateDocumentBase(docs);

        SearchServer server(idx);

        auto requests = converter.GetRequests();
        int limit = converter.GetResponsesLimit();

        // Получаем результаты в формате RelativeIndex
        auto results_rel = server.search(requests, limit);

        // Конвертируем в формат, который ждет putAnswers
        std::vector<std::vector<std::pair<int, float>>> results_converted;

        for (const auto& query_result : results_rel) {
            std::vector<std::pair<int, float>> converted_query;
            for (const auto& rel : query_result) {
                converted_query.emplace_back(static_cast<int>(rel.doc_id), rel.rank);
            }
            results_converted.push_back(converted_query);
        }

        // Теперь передаем конвертированный результат
        converter.putAnswers(results_converted);

        std::cout << "Search completed successfully" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
