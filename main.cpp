/*
COMSC-210 | Lab 25 | Rylan Der
IDE Used: Visual Studio Code
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

template <typename T> milliseconds Read(list<T>& list, ifstream& input);
template <typename T> milliseconds Read(set<T>& set, ifstream& input);
template <typename T> milliseconds Read(vector<T>& vector, ifstream& input);

template <typename T> milliseconds TimeSort(vector<T>& vector);
template <typename T> milliseconds TimeSort(list<T>& list);

template <typename T> milliseconds TimeInsert(vector<T>& vector, T value);
template <typename T> milliseconds TimeInsert(set<T>& set, T value);
template <typename T> milliseconds TimeInsert(list<T>& list, T value);

template <typename T> vector<milliseconds> ReadRace(list<T>& list, set<T>& set, vector<T>& vector, string filename);

int main() {
    const string FILENAME = "codes.txt";
    list<string> list;
    

    return 0;
}

/* syntax examples:
auto start = high_resolution_clock::now()
auto end = high_resolution_clock::now()
auto duration = duration_cast<milliseconds>(end - start)
duration.count() references elapsed milliseconds
*/

/**
 * Run race for read operations on given list, set vector
 * Returns vector of durations, ordered [0] list, [1] set, [2] vector
 * @param list List to read to
 * @param set Set to read to
 * @param vector Vector to read to
 * @param filename File to read data from
 * @return Vector of durations in milliseconds, ordered list, set, vector
 */
template <typename T> vector<milliseconds> ReadRace(list<T>& list, set<T>& set, vector<T>& vector, string filename) {
    vector<milliseconds> durations;

    //Verify file opens properly
    ifstream infile;
    infile.open(filename);
    if (!infile.is_open()) {
        cout << "Error opening file " << filename << endl;
        throw ios_base::failure("File open error");
    }

    //Time read operation for the list
    durations.push_back(Read(list, infile));

    //Reset file stream to beginning
    infile.clear();
    infile.seekg(0);

    //Time read operation for set
    durations.push_back(Read(set, infile));
    infile.clear();
    infile.seekg(0);

    //Time read operation for vector
    durations.push_back(Read(vector, infile));

    infile.close();

    return durations;
}
/**
 * Time how long it takes to read data into a list
 * @param list List to read data to
 * @param input Input stream to read from
 * @return Duration in milliseconds
 */
template <typename T> milliseconds Read(list<T>& list, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to list
    T value;
    while (getline(input, value)) {
        list.push_back(value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to read data into a set
 * @param set Set to read data to
 * @param input Input stream to read from
 * @return Duration in milliseconds
 */
template <typename T> milliseconds Read(set<T>& set, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to set
    T value;
    while (getline(input, value)) {
        set.insert(value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to read data into a vector
 * @param vector Vector to read data to
 * @param input Input stream to read from
 * @return Duration in milliseconds
 */
template <typename T> milliseconds Read(vector<T>& vector, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to vector
    T value;
    while (getline(input, value)) {
        vector.push_back(value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time sort function on given iterators
 * @param begin Iterator to beginning of ADT
 * @param end Iterator to end of ADT
 * @return Duration in milliseconds
 */
template <typename T> milliseconds TimeGeneralSort(T::iterator& begin, T::iterator& end) {
    auto start = high_resolution_clock::now();

    sort(begin, end);
    
    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time sort function of a list
 * @param list List to sort
 * @return Duration in milliseconds
 */
template <typename T> milliseconds TimeSort(list<T>& list) {
    auto start = high_resolution_clock::now();

    list.sort();

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to add a value to a set
 * @param set Set to insert a value to
 * @param value Value to insert into set
 * @return Duration in milliseconds
 */
template <typename T> milliseconds TimeInsertSet(set<T>& set, T value) {
    auto start = high_resolution_clock::now();

    set.insert(value);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to insert a value at the specified location
 * @param vector Vector to insert value to
 * @param location Iterator to insert value to
 * @param value Value to insert
 * @return Duration in milliseconds
 * @todo may want to change iterator to an index, and time the iteration duration as well
 */
template <typename T> milliseconds TimeInsertVector(vector<T>& vect, T::iterator& location, T value) {
    auto start = high_resolution_clock::now();

    vect.insert(location, value);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to insert a value at the specified location
 * @param vector Vector to insert value to
 * @param location Iterator to insert value to
 * @param value Value to insert
 * @return Duration in milliseconds
 * @todo may want to change iterator to an index, and time the iteration duration as well
 */
template <typename T> milliseconds TimeInsertList(list<T>& list, T::iterator& location, T value) {
    auto start = high_resolution_clock::now();

    list.insert(location, value);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}