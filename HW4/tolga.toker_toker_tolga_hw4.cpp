// Tolga Toker 32639
// CS300 Homework 4
// Main file

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>

using namespace std;

// Convert string to uppercase
string toupper(string str) {
    for (char& c : str) {
        c = toupper(c);
    }
    return str;
}

// PhoneBook Entry Structure
struct PhoneBookEntry {
    string name;
    string surname;
    string nameSurname;
    string telephone;
    string city;
};

// Template-based class for sorting and searching
template <typename T>
class PhoneBook {
public:
    // Constructor
    PhoneBook(const string& filename) {
        loadPhoneBook(filename);
    }

    // Insertion Sort
    void insertionSort() {
        auto start = chrono::high_resolution_clock::now();
        for (size_t i = 1; i < entries.size(); ++i) {
            T key = entries[i];
            int j = i - 1;
            while (j >= 0 && entries[j].nameSurname > key.nameSurname) {
                entries[j + 1] = entries[j];
                --j;
            }
            entries[j + 1] = key;
        }
        auto end = chrono::high_resolution_clock::now();
        insertionSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // Quick Sort (pivot = median)
    void quickSort(int low, int high) {
        auto start = chrono::high_resolution_clock::now();
        if (low < high) {
            if (high - low < 10) {
                insertionSort(low, high);
                return;
            }
            int pi = partition(low, high);
            quickSort(low, pi - 1);
            quickSort(pi + 1, high);
        }
        auto end = chrono::high_resolution_clock::now();
        quickSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // Merge Sort (in-place)
    void mergeSort(int low, int high) {
        auto start = chrono::high_resolution_clock::now();
        if (low < high) {
            int mid = low + (high - low) / 2;
            mergeSort(low, mid);
            mergeSort(mid + 1, high);
            merge(low, mid, mid + 1, high, high); // Pass 'high' as 'high2'
        }
        auto end = chrono::high_resolution_clock::now();
        mergeSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // Heap Sort
    void heapSort() {
        auto start = chrono::high_resolution_clock::now();
        buildHeap();
        for (int i = entries.size() - 1; i >= 0; --i) {
            swap(entries[0], entries[i]);
            heapify(0, i - 1);
        }
        auto end = chrono::high_resolution_clock::now();
        heapSortTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    }

    // Sequential Search
    vector<PhoneBookEntry> sequentialSearch(const string& query) {
        auto start = chrono::high_resolution_clock::now();
        vector<PhoneBookEntry> results;
        for (const auto& entry : entries) {
            if (entry.nameSurname.find(query) == 0) {
                results.push_back(entry);
            }
        }
        auto end = chrono::high_resolution_clock::now();
        sequentialSearchTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        return results;
    }

    // Binary Search
    vector<PhoneBookEntry> binarySearch(const string& query) {
        auto start = chrono::high_resolution_clock::now();
        vector<PhoneBookEntry> results;
        int low = 0;
        int high = entries.size() - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (entries[mid].nameSurname.find(query) == 0) {
                results.push_back(entries[mid]);
                // Check for duplicates
                int left = mid - 1;
                while (left >= 0 && entries[left].nameSurname.find(query) == 0) {
                    results.push_back(entries[left]);
                    --left;
                }
                int right = mid + 1;
                while (right < entries.size() && entries[right].nameSurname.find(query) == 0) {
                    results.push_back(entries[right]);
                    ++right;
                }
                break;
            } else if (entries[mid].nameSurname < query) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        auto end = chrono::high_resolution_clock::now();
        binarySearchTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        return results;
    }

    // Vector of PhoneBook Entries
    vector<PhoneBookEntry> entries;


    // Getters for sorting times
    long long getInsertionSortTime() const { return insertionSortTime; }
    long long getQuickSortTime() const { return quickSortTime; }
    long long getMergeSortTime() const { return mergeSortTime; }
    long long getHeapSortTime() const { return heapSortTime; }

    // Getters for searching times
    long long getSequentialSearchTime() const { return sequentialSearchTime; }
    long long getBinarySearchTime() const { return binarySearchTime; }

private:
    // Load PhoneBook from file
    void loadPhoneBook(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                PhoneBookEntry entry;
                size_t spacePos = line.find(' ');
                entry.name = toupper(line.substr(0, spacePos));
                line = line.substr(spacePos + 1);
                spacePos = line.find(' ');
                entry.surname = toupper(line.substr(0, spacePos));
                line = line.substr(spacePos + 1);
                entry.nameSurname = entry.name + " " + entry.surname;
                spacePos = line.find(' ');
                entry.telephone = line.substr(0, spacePos);
                entry.city = line.substr(spacePos + 1);
                entries.push_back(entry);
            }
            file.close();
        } else {
            cerr << "Error: Unable to open file " << filename << endl;
        }
    }

    // Partition for Quick Sort
    int partition(int low, int high) {
        T pivot = entries[low + (high - low) / 2];
        int i = low - 1;
        int j = high + 1;
        while (true) {
            do {
                ++i;
            } while (entries[i].nameSurname < pivot.nameSurname);
            do {
                --j;
            } while (entries[j].nameSurname > pivot.nameSurname);
            if (i >= j) {
                return j;
            }
            swap(entries[i], entries[j]);
        }
    }

    // Merge for Merge Sort
    void merge(int low, int mid, int high, int low2, int high2) {
        int i = low;
        int j = low2;
        int k = low;
        while (i <= mid && j <= high2) {
            if (entries[i].nameSurname <= entries[j].nameSurname) {
                entries[k] = entries[i];
                ++i;
            } else {
                entries[k] = entries[j];
                ++j;
            }
            ++k;
        }
        while (i <= mid) {
            entries[k] = entries[i];
            ++i;
            ++k;
        }
        while (j <= high2) {
            entries[k] = entries[j];
            ++j;
            ++k;
        }
    }

    // Build Heap for Heap Sort
    void buildHeap() {
        for (int i = entries.size() / 2 - 1; i >= 0; --i) {
            heapify(i, entries.size() - 1);
        }
    }

    // Heapify for Heap Sort
    void heapify(int i, int n) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left <= n && entries[left].nameSurname > entries[largest].nameSurname) {
            largest = left;
        }
        if (right <= n && entries[right].nameSurname > entries[largest].nameSurname) {
            largest = right;
        }
        if (largest != i) {
            swap(entries[i], entries[largest]);
            heapify(largest, n);
        }
    }

    // Insertion Sort for small subarrays in Quick Sort
    void insertionSort(int low, int high) {
        for (int i = low + 1; i <= high; ++i) {
            T key = entries[i];
            int j = i - 1;
            while (j >= low && entries[j].nameSurname > key.nameSurname) {
                entries[j + 1] = entries[j];
                --j;
            }
            entries[j + 1] = key;
        }
    }

    // Sorting times
    long long insertionSortTime = 0;
    long long quickSortTime = 0;
    long long mergeSortTime = 0;
    long long heapSortTime = 0;

    // Searching times
    long long sequentialSearchTime = 0;
    long long binarySearchTime = 0;
};

int main() {
    string filename;
    string query;
    string queryupper;

    cout << "Please enter the contact file name:" << endl;
    cin >> filename;

    // query could consist of multiple words separated by a space character or a single word
    cout << "Please enter the word to be queried:" << endl;
    cin.ignore();
    getline(cin, query);

    queryupper = toupper(query);

    // Create PhoneBook objects for each sorting algorithm
    PhoneBook<PhoneBookEntry> phoneBookQuickSort(filename);
    PhoneBook<PhoneBookEntry> phoneBookInsertionSort(filename);
    PhoneBook<PhoneBookEntry> phoneBookMergeSort(filename);
    PhoneBook<PhoneBookEntry> phoneBookHeapSort(filename);

    // Sort the PhoneBook copies
    cout << "Sorting the vector copies\n======================================\n";
    phoneBookQuickSort.quickSort(0, phoneBookQuickSort.entries.size() - 1);
    cout << "Quick Sort Time: " << phoneBookQuickSort.getQuickSortTime() << " Nanoseconds\n";
    phoneBookInsertionSort.insertionSort();
    cout << "Insertion Sort Time: " << phoneBookInsertionSort.getInsertionSortTime() << " Nanoseconds\n";
    phoneBookMergeSort.mergeSort(0, phoneBookMergeSort.entries.size() - 1);
    cout << "Merge Sort Time: " << phoneBookMergeSort.getMergeSortTime() << " Nanoseconds\n";
    phoneBookHeapSort.heapSort();
    cout << "Heap Sort Time: " << phoneBookHeapSort.getHeapSortTime() << " Nanoseconds\n";

    // Search for the query
    cout << "\nSearching for " << query << "\n======================================";
    vector<PhoneBookEntry> binarySearchResults = phoneBookHeapSort.binarySearch(queryupper);

    // Print search results for Binary Search
    cout << "\nSearch results for Binary Search:\n";
    if (binarySearchResults.empty()) {
        cout << query << " does NOT exist in the dataset\n";
    } else {
        for (const auto& entry : binarySearchResults) {
            cout << entry.name << " " << entry.surname << " " << entry.telephone << " " << entry.city << endl;
        }
    }
    cout << "\nBinary Search Time: " << fixed << setprecision(2) << phoneBookHeapSort.getBinarySearchTime() << " Nanoseconds\n";

    // Print search results for Sequential Search
    cout << "\nSearch results for Sequential Search:\n";
    vector<PhoneBookEntry> sequentialSearchResults = phoneBookHeapSort.sequentialSearch(queryupper);
    if (sequentialSearchResults.empty()) {
        cout << query << " does NOT exist in the dataset\n";
    } else {
        for (const auto& entry : sequentialSearchResults) {
            cout << entry.name << " " << entry.surname << " " << entry.telephone << " " << entry.city << endl;
        }
    }
    cout << "\nSequential Search Time: " << fixed << setprecision(2) << phoneBookHeapSort.getSequentialSearchTime() << " Nanoseconds\n";

    // Calculate and print speedups
    cout << "\nSpeedUp between Search Algorithms\n======================================\n";
    if (phoneBookHeapSort.getSequentialSearchTime() > phoneBookHeapSort.getBinarySearchTime()) {
        cout << "(Sequential Search/ Binary Search) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookHeapSort.getSequentialSearchTime() / phoneBookHeapSort.getBinarySearchTime() << endl;
    } else {
        cout << "(Binary Search/ Sequential Search) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookHeapSort.getBinarySearchTime() / phoneBookHeapSort.getSequentialSearchTime() << endl;
    }

    cout << "\nSpeedUps between Sorting Algorithms\n======================================\n";
    if (phoneBookInsertionSort.getInsertionSortTime() > phoneBookQuickSort.getQuickSortTime()) {
        cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookInsertionSort.getInsertionSortTime() / phoneBookQuickSort.getQuickSortTime() << endl;
    } else {
        cout << "(Quick Sort/ Insertion Sort) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookQuickSort.getQuickSortTime() / phoneBookInsertionSort.getInsertionSortTime() << endl;
    }
    if (phoneBookMergeSort.getMergeSortTime() > phoneBookQuickSort.getQuickSortTime()) {
        cout << "(Merge Sort / Quick Sort) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookMergeSort.getMergeSortTime() / phoneBookQuickSort.getQuickSortTime() << endl;
    } else {
        cout << "(Quick Sort/ Merge Sort) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookQuickSort.getQuickSortTime() / phoneBookMergeSort.getMergeSortTime() << endl;
    }
    if (phoneBookHeapSort.getHeapSortTime() > phoneBookQuickSort.getQuickSortTime()) {
        cout << "(Heap Sort / Quick Sort) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookHeapSort.getHeapSortTime() / phoneBookQuickSort.getQuickSortTime() << endl;
    } else {
        cout << "(Quick Sort/ Heap Sort) SpeedUp = " << fixed << setprecision(4) << (double)phoneBookQuickSort.getQuickSortTime() / phoneBookHeapSort.getHeapSortTime() << endl;
    }

    return 0;
}