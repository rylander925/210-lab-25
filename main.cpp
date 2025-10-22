/*
COMSC-210 | Lab 25 | Rylan Der
IDE Used: Visual Studio Code
*/

#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

template <typename T> void Read(T::iterator& begin, T::iterator&, istream& input);
template <typename T> milliseconds TimeGeneralSort(T::iterator& begin, T::iterator& end);
template <typename T> milliseconds TimeListSort(list<T>& list);
template <typename T> milliseconds TimeInsertVector(vector<T>& vector, T value);
template <typename T> milliseconds TimeInsertSet(set<T>& set, T value);
template <typename T> milliseconds TimeGeneralInsert(T::iterator& location, T value);

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
 * Read data from an input stream into a ADT using iterators
 * @param begin Iterator to beginning of ADT
 * @param end Iterator to end of ADT
 * @param input Input stream to read into ADT
 */
template <typename T> void Read(T::iterator& begin, T::iterator& end, istream& input) {
    for (T::iterator it = begin; it != end; it++) {
        input >> *it;
    }
}

/**
 * Time sort function on given iterators
 * @param begin Iterator to beginning of ADT
 * @param end Iterator to end of ADT
 * @return Duration in milliseconds
 */
template <typename T> milliseconds TimeGeneralSort(T::iterator& begin, T::iterator& end) {

}

/**
 * Time sort function of a list
 * @param list List to sort
 * @return Duration in milliseconds
 */
template <typename T> milliseconds TimeListSort(list<T>& list) {

}

/**
 * Time how long it takes to add a value to a set
 * @param set Set to insert a value to
 * @param value Value to insert into set
 * @return Duration in milliseconds
 */
template <typename T> milliseconds TimeInsertSet(set<T>& set, T value) {

}

/**
 * Time how long it takes to insert a value at the specified location
 * @param location Iterator to insert value to
 * @param value Value to insert
 * @return Duration in milliseconds
 */
template <typename T> milliseconds TimeGeneralInsert(T::iterator& location, T value) {

}