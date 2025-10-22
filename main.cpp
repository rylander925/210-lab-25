/*
COMSC-210 | Lab 25 | Rylan Der
IDE Used: Visual Studio Code
*/

#include <iostream>
#include <iomanip>
#include <istream>
#include <fstream>
#include <vector>
#include <set>
#include <list>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

template <typename T> milliseconds Read(T::iterator& begin, T::iterator& end, istream* input);
template <typename T> milliseconds TimeGeneralSort(T::iterator& begin, T::iterator& end);
template <typename T> milliseconds TimeListSort(list<T>& list);
template <typename T> milliseconds TimeInsertVector(vector<T>& vector, T value);
template <typename T> milliseconds TimeInsertSet(set<T>& set, T value);
template <typename T> milliseconds TimeInsertList(T::iterator& location, T value);
template <typename T> milliseconds ReadCodes(T::iterator& begin, T::iterator& end, string filename);

int main() {
    

    return 0;
}

/* syntax examples:
auto start = high_resolution_clock::now()
auto end = high_resolution_clock::now()
auto duration = duration_cast<milliseconds>(end - start)
duration.count() references elapsed milliseconds
*/

/**
 * Open code file and read the codes into given ADT
 * @param begin Iterator to beginning of ADT
 * @param end Iterator to end of ADT
 * @param filename String to read codes from
 * @return Duration in milliseconds
 */
template <typename T> milliseconds ReadCodes(T::iterator& begin, T::iterator& end, string filename) {
    ifstream infile;
    infile.open(filename);
    if (!infile.is_open()) {
        cout << "Error opening file " << filename << endl;
        throw ios_base::failure("File open error");
    }

    istream *input = &infile;

    return Read(begin, end, input);
}

/**
 * Time how long it takes to read data from an input stream into a ADT using iterators
 * @param begin Iterator to beginning of ADT
 * @param end Iterator to end of ADT
 * @param input Input stream to read into ADT
 * @return Duration in milliseconds
 */
template <typename T> milliseconds Read(T::iterator& begin, T::iterator& end, istream* input) {
    auto start = high_resolution_clock::now();

    for (T::iterator it = begin; it != end; it++) {
        getline(*input, *it);
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
template <typename T> milliseconds TimeListSort(list<T>& list) {
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