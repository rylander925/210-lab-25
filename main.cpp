/*
COMSC-210 | Lab 25 | Rylan Der
IDE Used: Visual Studio Code
*/

#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

template <typename T> milliseconds Read(T::iterator& begin, T::iterator& end);
template <typename T> milliseconds GeneralSort(T::iterator& begin, T::iterator& end);
template <typename T> milliseconds ListSort(list<T>& list);
template <typename T> milliseconds InsertVector(vector<T>& vector, T value);
template <typename T> milliseconds InsertSet(set<T>& set, T value);
template <typename T> milliseconds GeneralInsert(T::iterator& location, T value);

int main() {
    

    return 0;
}

/* syntax examples:
auto start = high_resolution_clock::now()
auto end = high_resolution_clock::now()
auto duration = duration_cast<milliseconds>(end - start)
duration.count() references elapsed milliseconds
*/