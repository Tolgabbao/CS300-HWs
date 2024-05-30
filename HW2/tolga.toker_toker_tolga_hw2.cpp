// Tolga Toker 32639
// CS300 Homework 2
// main.cpp file

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include "avl_tree.h"

using namespace std;

//lowers the input
string to_lower(string word){
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word;
}


// Convert query string to a vector of words
vector<string> queryToWord(const string& query) {
    vector<string> wordVector;
    string word;
    for (char c : query) {
        if (c == ' ') {
            wordVector.push_back(word);
            word = "";
        } else {
            word += c;
        }
    }
    wordVector.push_back(word);

    return wordVector;
}


// Count how many words
int countWords(string filename, vector<DocumentItem>* searchVector) {
    int countOfWord = 0;
    for (int i = 0; i < searchVector->size(); i++) {
        if (filename == searchVector->at(i).documentName) {
            countOfWord = searchVector->at(i).count;
            break;
        }
    }
    return countOfWord;
}



int main() {
    int numDoc; // number of documents that are going to be preprocessed
    string sNumDoc;
    cout << "Enter number of input files: ";
    getline(cin, sNumDoc); //get the inputs using getline
    numDoc = stoi(sNumDoc); //turn the string into an int

    //cin.ignore();

    vector<string> docNames; //stores document names
    for (int i = 0; i < numDoc; i++) {
        string docName;
        cout << "Enter " + to_string(i + 1) + ". file name: ";
        cin >> docName; //get the document names from the user
        docNames.push_back(docName); //push them to the vector
    }

    // PREPROCESS

    // creating a tree
    AVLSearchTree<string, vector<DocumentItem> * >  * avlTree = new AVLSearchTree<string, vector<DocumentItem> * > (NULL);
    for (int i = 0; i < numDoc; i++) { //loop through all your documents
        string docName = docNames[i];
        ifstream docFile;
        docFile.open(docName);
        if (docFile.is_open()) { // check if the file is successfully opened
            string word;
            while (docFile >> word) { //read the word from the file
                word = to_lower(word);
                if(avlTree->find(word) == nullptr) { //if the word does not already exist,insert it
                    vector<DocumentItem> * docItemVector = new vector<DocumentItem>;
                    DocumentItem docItem(docName, 1);
                    docItemVector->push_back(docItem);
                    avlTree->insert(word, docItemVector);
                }
                else{ //if the word already exists in the tree
                    bool found = false;
                    vector<DocumentItem>* docItemVector = avlTree->find(word);
                    for (int j = 0; j < docItemVector->size(); j++) {
                        if (docItemVector->at(j).documentName == docName) {
                            // Found the document in the vector, update the count
                            docItemVector->at(j).count++;
                            found = true;  // Set the flag to true since we found and updated the document
                            break;
                        }
                    }
                    // If the document is not found in the vector, add a new DocumentItem
                    if (!found) {
                        DocumentItem docItem(docName, 1);
                        docItemVector->push_back(docItem);
                    }
                }
            }
            docFile.close();
        }
        else {
            cout << "Failed to open file: " << docName << endl;
            return 1; // return an error code
        }
    }
    string wordQuery;
    cin.ignore(); // Ignore newline character from previous input
    while (wordQuery != "ENDOFINPUT") {
        cout << endl;
        string wordQ;
        cout << "Enter queried words in one line: ";
        getline(cin, wordQuery); //get the query from the user
        vector<string> wordVector = queryToWord(wordQuery);
        if (wordVector.at(0) == "ENDOFINPUT") {
            break;
        }
        // Check if the query is for removal
        if (wordVector.at(0) == "REMOVE") {
            for (int i = 1; i < wordVector.size(); i++) {
                string wordToRemove = to_lower(wordVector.at(i));
                avlTree->remove(wordToRemove); //if so remove the element from the tree
                cout << wordToRemove + " has been REMOVED" << endl;
            }
        }
        else { //if not for removal
            bool noOutput = true;
            bool notIn = false;
            for (int j = 0; j < docNames.size(); j++) {
                string doc = docNames[j];
                if (notIn) {
                    break;
                }
                string result;
                for (int i = 0; i < wordVector.size(); i++) { //iterate for all the words
                    wordQ = to_lower(wordVector.at(i));
                    vector <DocumentItem>* info = avlTree->find(wordQ); // info points to the document item vector of our word
                    if (info != nullptr) { // if the word is in the tree
                        int count = countWords(doc, info); // find the count
                        if (count == 0) { // this word is not in this document
                            result = "";
                            break;
                        }
                        else {
                            result += wordQ + " found " + to_string(count) + " times ";
                        }
                    }
                    else { //if the word is not in the tree -> no document would have it
                        result = "";
                        notIn = true;
                        break;
                    }
                }
                if (result != "") {
                    cout << "in Document " << doc << " " << result << endl;
                    noOutput = false;
                }
            }
            if (noOutput) {
                cout << "No document contains the query " << endl;
            }
        }
    };
    return 0;
}

