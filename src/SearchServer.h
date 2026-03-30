#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#include "InvertedIndex.h"
#include <vector>
#include <string>

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator == (const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex& idx);

    // Метод должен принимать const std::vector<std::string>& и int
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input, int limit);

private:
    InvertedIndex _index;
};

#endif //SEARCHSERVER_H