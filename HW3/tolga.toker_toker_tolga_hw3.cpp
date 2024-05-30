// Tolga Toker 32639
// CS300 Homework 3
// Main file

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <map>
#include "avl_tree.h"
#include "hash_table.h"

using namespace std;

// Function to preprocess a document and insert words into data structures
void preprocessDocument(const string &filename, AVLSearchTree<string, WordItem> &avlTree, HashTable<WordItem> &hashTable)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string word;
        while (ss >> word)
        {
            // Convert word to lowercase
            transform(word.begin(), word.end(), word.begin(), ::tolower);

            // Split words by non-alphabetic characters, replace with space
            for (char &c : word)
            {
                if (!isalpha(c))
                {
                    c = ' ';
                }
            }

            // Split words by space
            stringstream wordStream(word);
            while (wordStream >> word)
            {
                // Remove non-alphabetic characters
                word.erase(remove_if(word.begin(), word.end(), [](char c)
                                     { return !isalpha(c); }),
                           word.end());
                if (!word.empty())
                {
                    // Insert word into AVL tree
                    if (avlTree.contains(word))
                    {
                        WordItem &existingWordItem = const_cast<WordItem &>(avlTree.find(word));
                        bool documentFound = false;
                        for (DocumentItem &docItem : existingWordItem.documentItems)
                        {
                            if (docItem.documentName == filename)
                            {
                                docItem.count++;
                                documentFound = true;
                                break;
                            }
                        }
                        if (!documentFound)
                        {
                            existingWordItem.documentItems.push_back(DocumentItem(filename, 1));
                        }
                    }
                    else
                    {
                        WordItem newWordItem;
                        newWordItem.word = word;
                        newWordItem.documentItems.push_back(DocumentItem(filename, 1));
                        avlTree.insert(word, newWordItem);
                    }

                    // Insert word into hash table
                    WordItem hashWordItem;
                    hashWordItem.word = word;
                    const WordItem &foundWordItem = hashTable.find(hashWordItem);
                    if (foundWordItem.word == word)
                    {
                        WordItem &existingHashWordItem = const_cast<WordItem &>(foundWordItem);
                        bool documentFound = false;
                        for (DocumentItem &docItem : existingHashWordItem.documentItems)
                        {
                            if (docItem.documentName == filename)
                            {
                                docItem.count++;
                                documentFound = true;
                                break;
                            }
                        }
                        if (!documentFound)
                        {
                            existingHashWordItem.documentItems.push_back(DocumentItem(filename, 1));
                        }
                    }
                    else
                    {
                        hashWordItem.documentItems.push_back(DocumentItem(filename, 1));
                        hashTable.insert(hashWordItem);
                    }
                }
            }
        }
    }

    file.close();
}

// Function to query documents for given words using AVL tree
vector<WordItem> queryDocumentsAVL(const AVLSearchTree<string, WordItem> &avlTree, const vector<string> &queryWords)
{
    vector<WordItem> result;
    for (const string &queryWord : queryWords)
    {
        if (avlTree.contains(queryWord))
        {
            result.push_back(avlTree.find(queryWord));
        }
    }
    return result;
}

// Function to query documents for given words using hash table
vector<WordItem> queryDocumentsHash(const HashTable<WordItem> &hashTable, const vector<string> &queryWords)
{
    vector<WordItem> result;
    for (const string &queryWord : queryWords)
    {
        WordItem searchWordItem;
        searchWordItem.word = queryWord;
        const WordItem &foundWordItem = hashTable.find(searchWordItem);
        if (foundWordItem.word == queryWord)
        {
            result.push_back(foundWordItem);
        }
    }
    return result;
}

// Helper function to get a list of unique document names from the result
vector<string> getDocumentNames(const vector<WordItem> &result)
{
    vector<string> documentNames;
    for (const WordItem &wordItem : result)
    {
        for (const DocumentItem &docItem : wordItem.documentItems)
        {
            if (find(documentNames.begin(), documentNames.end(), docItem.documentName) == documentNames.end())
            {
                documentNames.push_back(docItem.documentName);
            }
        }
    }
    return documentNames;
}

// Helper function to check if all query words are present in a document
bool allQueryWordsPresent(const vector<WordItem> &result, const vector<string> &queryWords, const string &docName)
{
    for (const string &queryWord : queryWords)
    {
        bool wordFound = false;
        for (const WordItem &wordItem : result)
        {
            if (wordItem.word == queryWord)
            {
                for (const DocumentItem &docItem : wordItem.documentItems)
                {
                    if (docItem.documentName == docName)
                    {
                        wordFound = true;
                        break;
                    }
                }
                if (wordFound)
                {
                    break;
                }
            }
        }
        if (!wordFound)
        {
            return false; // Not all query words are present
        }
    }
    return true; // All query words are present
}

void printQueryResults(const vector<WordItem> &result, const vector<string> &queryWords)
{
    // Iterate through each document name in the result
    bool firstWord = false;
    for (const string &docName : getDocumentNames(result))
    {
        // Check if all query words are present in the document
        if (allQueryWordsPresent(result, queryWords, docName))
        {
            firstWord = true;
            cout << "in Document " << docName;
            // Print the word and count for each query word in the document
            for (const WordItem &wordItem : result)
            {
                for (const DocumentItem &docItem : wordItem.documentItems)
                {
                    if (docItem.documentName == docName)
                    {
                        cout << ", " << wordItem.word << " found " << docItem.count << " times";
                    }
                }
            }
            cout << "." << endl;
        }
    }


    // If no document contains all the query words, print a message.
    if (!firstWord)
            cout << "No document contains all the given query words" << endl;
}

int main()
{
    int numFiles;
    cout << "Enter number of input files: ";
    cin >> numFiles;

    vector<string> filenames;
    for (int i = 0; i < numFiles; i++)
    {
        string filename;
        cout << "Enter " << i + 1 << ". file name: ";
        cin >> filename;
        filenames.push_back(filename);
    }

    AVLSearchTree<string, WordItem> avlTree{WordItem()}; // Use curly braces
    HashTable<WordItem> hashTable;

    // Preprocess documents
    for (const string &filename : filenames)
    {
        preprocessDocument(filename, avlTree, hashTable);
    }

    cout << "\nAfter preprocessing, the unique word count is " << hashTable.getCurrentSize() << ". Current load ratio is " << hashTable.getLoadFactor() << endl;

    string queryLine;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, queryLine);

    stringstream queryStream(queryLine);
    string queryWord;
    vector<string> queryWords;
    while (queryStream >> queryWord)
    {
        // Convert query word to lowercase
        transform(queryWord.begin(), queryWord.end(), queryWord.begin(), ::tolower);

        // Change non-alphabetic characters to space
        for (char &c : queryWord)
        {
            if (!isalpha(c))
            {
                c = ' ';
            }
        }

        // Split query word by space
        stringstream wordStream(queryWord);
        queryWord.clear();
        while (wordStream >> queryWord)
        {
            // Remove non-alphabetic characters
            queryWord.erase(remove_if(queryWord.begin(), queryWord.end(), [](char c)
                                      { return !isalpha(c); }),
                            queryWord.end());
            if (!queryWord.empty())
            {
                queryWords.push_back(queryWord);
            }
        }
    }

    // Query documents using AVL tree
    auto avlStart = chrono::high_resolution_clock::now();
    for (int i = 0; i < 20; i++)
    {
        vector<WordItem> avlResult = queryDocumentsAVL(avlTree, queryWords);
    }
    auto avlEnd = chrono::high_resolution_clock::now();
    chrono::duration<double, nano> avlTime = avlEnd - avlStart;

    // Query documents using hash table
    auto hashStart = chrono::high_resolution_clock::now();
    for (int i = 0; i < 20; i++)
    {
        vector<WordItem> hashResult = queryDocumentsHash(hashTable, queryWords);
    }
    auto hashEnd = chrono::high_resolution_clock::now();
    chrono::duration<double, nano> hashTime = hashEnd - hashStart;

    vector<WordItem> avlResult = queryDocumentsAVL(avlTree, queryWords);
    vector<WordItem> hashResult = queryDocumentsHash(hashTable, queryWords);

    printQueryResults(avlResult, queryWords);
    printQueryResults(hashResult, queryWords);

    // Print results
    cout << "\nTime: " << avlTime.count() / 20 << endl;
    cout << "Time: " << hashTime.count() / 20 << endl;
    cout << "Speed Up: " << avlTime.count() / hashTime.count() << endl;

    return 0;
}
