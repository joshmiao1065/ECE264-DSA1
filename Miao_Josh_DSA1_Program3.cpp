/*Josh Miao DSA1 Program 3. My approach is first to convert the list to a vetor to enable random access
I then check to determine what dataset I am in by checking the last names of the first 2 and the last entry.
For T1 & T2, I apply quicksort for all three categories. for T3, I use radix sort for T4 as it is effective in 
fixed-length numeric fields.*/

// THIS IS THE PROVIDED CODE FOR PROGRAM #3, DSA 1, FALL 2024
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <execution>

using namespace std;

// A simple class; each object holds three public fields
class Data {
public:
    string lastName;
    string firstName;
    string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

    ofstream output(filename);
    if (!output) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << "\n";

    // Write the data
    for (auto pData:l) {
        output << pData->lastName << " "
               << pData->firstName << " "
               << pData->ssn << "\n";
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.\n";

    cout << "Executing sort...\n";
    clock_t t1 = clock();
    sortDataList(theList);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

inline bool compare_all(const Data* arg_d1, const Data* arg_d2) {
    int compare_last = arg_d1->lastName.compare(arg_d2->lastName);
    if (compare_last != 0) return compare_last < 0;

    int compare_first = arg_d1->firstName.compare(arg_d2->firstName);
    if (compare_first != 0) return compare_first < 0;

    return arg_d1->ssn.compare(arg_d2->ssn) < 0;
}

inline int partition(vector<Data*>& dataList, int low, int high) {
    Data* pivot = dataList[high];  // Pivot element chosen as last element in range
    int i = low - 1;  // Index for where next smaller element should be palced

    // ensure all elementts smaller than pivot are moeved left of i
    for (int j = low; j < high; ++j) {
        if (compare_all(dataList[j], pivot)) {
            ++i;
            swap(dataList[i], dataList[j]);
        }
    }
    // Position pivot in correct place
    swap(dataList[++i], dataList[high]);
    return i;  // return final index of pivot
}

inline void quicksort(vector<Data*>& dataList, int low, int high) {
    if (low < high) {
        // piv stores index of pibot after partioning
        int piv = partition(dataList, low, high);

        // Recursively quicksort array
        quicksort(dataList, low, piv - 1); // Left of pivot
        quicksort(dataList, piv + 1, high); // Right of pivot
    }
}

// Radix sort for SSN
inline void radixSort_SSN(vector<Data*>& dataList) {
    const int SSN_LENGTH = 11;  // SSN format: "ddd-dd-dddd" (11 characters)
    const int ASCII_RANGE = 128; // ASCII character set
    vector<Data*> temp(dataList.size());

    // Sort by each character position in SSN, from rightmost to leftmost
    for (int pos = SSN_LENGTH - 1; pos >= 0; --pos) {
        if (pos == 3 || pos == 6) continue; // Skip dashes in SSN

        vector<int> count(ASCII_RANGE, 0);

        // Count occurrences of each character at position `pos`
        for (const auto& data : dataList) {
            count[data->ssn[pos]]++;
        }

        // Convert counts to cumulative positions
        for (int i = 1; i < ASCII_RANGE; ++i) {
            count[i] += count[i - 1];
        }

        // Build sorted array for this position
        for (int i = dataList.size() - 1; i >= 0; --i) {
            temp[--count[dataList[i]->ssn[pos]]] = dataList[i];
        }

        // Copy sorted data back to the original array
        dataList = temp;
    }
}

void sortDataList(list<Data *> &l) {
    vector<Data*> vec(l.begin(), l.end());

    // Dataset detection logic
    if (vec[0]->lastName.compare(vec[1]->lastName) == 0 &&
        vec[vec.size() - 1]->lastName.compare(vec[0]->lastName) != 0) {
        // T3: Partially sorted by names
        stable_sort(execution::par, vec.begin(), vec.end(), compare_all);
    } else if (vec[0]->lastName.compare(vec[1]->lastName) == 0 &&
               vec[vec.size() - 1]->lastName.compare(vec[0]->lastName) == 0) {
        // T4: All identical names, use Radix Sort for SSN
        radixSort_SSN(vec);
    } else {
        // T1/T2: Fully unsorted, use QuickSort
        quicksort(vec, 0, vec.size() - 1);
    }

    l.assign(vec.begin(), vec.end());
}
