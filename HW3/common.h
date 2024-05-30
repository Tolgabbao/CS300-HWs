// Tolga Toker 32639
// CS300 Homework 3
// common.h
#ifndef common_h
#define common_h

#include <string>
#include <vector>

struct DocumentItem {
    std::string documentName;
    int count;
    DocumentItem(std::string docName, int c) : documentName(docName), count(c) {}
};

struct WordItem {
    std::string word;
    std::vector<DocumentItem> documentItems;
};

#endif // common_h