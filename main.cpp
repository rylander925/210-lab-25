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
#include <string>
using namespace std;
using namespace chrono;

const int DEFAULT_SPACING = 10;

microseconds Read(list<string>& list, ifstream& input);
microseconds Read(set<string>& set, ifstream& input);
microseconds Read(vector<string>& vect, ifstream& input);

microseconds TimeSort(vector<string>& vector, int tests);
microseconds TimeSort(list<string>& list, int tests);

microseconds TimeInsert(vector<string>& vector, int index, string value, int tests);
microseconds TimeInsert(set<string>& set, string value, int tests);
microseconds TimeInsert(list<string>& list, int index, string value, int tests);

microseconds TimeDelete(vector<string>& vector, int index, int tests);
microseconds TimeDelete(set<string>& set, int index, int tests);
microseconds TimeDelete(list<string>& list, int index, int tests);

vector<microseconds> ReadRace(list<string>& list, vector<string>& vect, set<string>& set, string filename, int tests);
vector<microseconds> SortRace(list<string>& list, vector<string>& vect, int tests);
vector<microseconds> InsertRace(list<string>&list, vector<string>& vect, set<string>& set, string value, int tests);
vector<microseconds> DeleteRace(list<string>&list, vector<string>& vect, set<string>& set, int tests);

void OutputRace(vector<microseconds> durations, string raceName, int spacing = DEFAULT_SPACING);
void OutputRace(vector<string> names, int spacing = DEFAULT_SPACING);

int main() {
    const string FILENAME = "codes.txt";
    const int READ_TESTS = 1;
    const int SORT_TESTS = 1;
    const int INSERTION_TESTS = 1;
    const int DELETION_TESTS = 1;
    list<string> list;
    set<string> set;
    vector<string> vect;
    
    OutputRace(vector<string>{"List", "Vector", "Set"});
    OutputRace(ReadRace(list, vect, set, FILENAME, READ_TESTS), "Read");
    OutputRace(SortRace(list, vect, SORT_TESTS), "Sort");
    OutputRace(InsertRace(list, vect, set, "TESTCODE", INSERTION_TESTS), "Insert");
    OutputRace(DeleteRace(list, vect, set, DELETION_TESTS), "Delete");

    return 0;
}

/* syntax examples:
auto start = high_resolution_clock::now()
auto end = high_resolution_clock::now()
auto duration = duration_cast<microseconds>(end - start)
duration.count() references elapsed microseconds
*/

/**
 * Outputs race names as a header table row
 * @param names     Names to be displayed above table columns
 * @param spacing   Spacing between table columns 
 */
void OutputRace(vector<string> names, int spacing) {
    //Leave empty space for name column
    cout << left << setw(spacing) << "";

    //followed by titles for data columns
    for (string name : names) {
        cout << left << setw(spacing) << name;
    }
    cout << endl;
}


/**
 * Outputs race durations as a table row: name followed by durations separated by spaces
 * @param durations List of durations, ordered list, vector, set
 * @param raceName  Name to be displayed to the left of the durations
 * @param spacing   Spacing between table columns 
 */
void OutputRace(vector<microseconds> durations, string raceName, int spacing) {
    //output race name
    cout << left << setw(spacing) << raceName;

    //followed by entries
    for (microseconds duration : durations) {
        cout << left << setw(spacing) << duration.count();
    }
    cout << endl;
}

/**
 * Returns durations of deletion operations for a list, vector, and a set
 * @param testList List to delete from
 * @param vect     Vector to delete from
 * @param set      Set to delete from
 * @param tests    Number of times to repeat test
 * @return Vector containing durations of tests, ordered list, vector, set
 */
vector<microseconds> DeleteRace(list<string>&testList, vector<string>& vect, set<string>& set, int tests) {
    //Run timers and return as an array
    return vector<microseconds>
        {   TimeDelete(testList, testList.size() / 2, tests),
            TimeDelete(vect, vect.size() / 2, tests),
            TimeDelete(set, set.size() / 2, tests)
        };
}

/**
 * Returns durations of insertion operations for a list, vector, and a set
 * @param testList List to insert to
 * @param vect     Vector to insert to
 * @param set      Set to insert to
 * @param value    Value to insert
 * @param tests    Number of times to repeat test
 * @return Vector containing durations of tests, ordered list, vector, set
 */
vector<microseconds> InsertRace(list<string>&testList, vector<string>& vect, set<string>& set, string value, int tests) {
    //Run timers and return as an array
    return vector<microseconds>
        {   TimeInsert(testList, testList.size() / 2, value, tests),
            TimeInsert(vect, vect.size() / 2, value, tests),
            TimeInsert(set, value, tests)
        };
}

/**
 * Returns resulting durations from sorting race between a list and vector. Third parameter (for a set) is -1.
 * @param list  List to sort
 * @param vect  Vector to sort
 * @param tests Number of times to repeat test
 * @return Vector of sorting durations, ordered list, vector, set
 */
vector<microseconds> SortRace(list<string>& list, vector<string>& vect, int tests) {
    //Run timers and return as an array. The third parameter is set to negative one for a set
    return vector<microseconds>{TimeSort(list, tests), TimeSort(vect, tests), static_cast<microseconds>(-1)};
}

/**
 * Run race for read operations on given list, vector, and set
 * Returns vector of durations, ordered [0] list, [1] vector, [2] set
 * @param list List to read to
 * @param set Set to read to
 * @param vector Vector to read to
 * @param filename File to read data from
 * @return Vector of durations in microseconds, ordered list, vector, set
 */
vector<microseconds> ReadRace(list<string>& testList, vector<string>& testVector, set<string>& testSet, string filename, int tests) {
    vector<microseconds> durations(3, microseconds(0));
    list<string> dummyList;
    vector<string> dummyVect;
    set<string> dummySet;

    //Verify file opens properly
    ifstream infile;
    infile.open(filename);
    if (!infile.is_open()) {
        cout << "Error opening file " << filename << endl;
        throw ios_base::failure("File open error");
    }

    for (int i = 0; i < tests; i++) {
        //Time read operation for the list
        //Use a dummy list for repeat tests
        durations.at(0) += Read((i == 0 ? testList : dummyList), infile); 

        //Reset file stream to beginning
        infile.clear();
        infile.seekg(0);

        //Time read operation for vector
        durations.at(1) += Read((i == 0) ? testVector : dummyVect, infile);
        infile.clear();
        infile.seekg(0);
        
        //Time read operation for set
        durations.at(2) += Read((i == 0) ? testSet : dummySet, infile);
        infile.clear();
        infile.seekg(0);

        //clear dummys for repeated tests
        dummyList.clear();
        dummyVect.clear();
        dummySet.clear();
    }

    infile.close();

    return durations;
}

/**
 * Time how long it takes to read data into a list
 * @param list List to read data to
 * @param input Input stream to read from
 * @return Duration in microseconds 
 */
microseconds Read(list<string>& list, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to list
    string value;
    while (getline(input, value)) {
        list.push_back(value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time how long it takes to read data into a set
 * @param set Set to read data to
 * @param input Input stream to read from
 * @return Duration in microseconds
 */
microseconds Read(set<string>& set, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to set
    string value;
    while (getline(input, value)) {
        set.insert(value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time how long it takes to read data into a vector
 * @param vector Vector to read data to
 * @param input Input stream to read from
 * @return Duration in microseconds
 */
microseconds Read(vector<string>& vector, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to vector
    string value;
    while (getline(input, value)) {
        vector.push_back(value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time sort function of a list
 * @param list  List to sort
 * @param tests Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeSort(list<string>& l, int tests) {
    microseconds totalTime(0);
    for (int i = 0; i < tests; i++) {
        list<string> testList = l;

        auto start = high_resolution_clock::now();
        testList.sort();
        auto end = high_resolution_clock::now();

        totalTime += duration_cast<microseconds>(end - start);
    }
    return totalTime;
}

/**
 * Time sort function of a vector
 * @param vector Vector to sort
 * @param tests  Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeSort(vector<string>& vect, int tests) {
    microseconds totalTime(0);
    for (int i = 0; i < tests; i++) {
        vector<string> testVect = vect;

        auto start = high_resolution_clock::now();
        sort(testVect.begin(), testVect.end());
        auto end = high_resolution_clock::now();

        totalTime += duration_cast<microseconds>(end - start);
    }
    return totalTime;
}

/**
 * Time how long it takes to add a value to a set
 * @param set   Set to insert a value to
 * @param value Value to insert into set
 * @param tests Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeInsert(set<string>& set, string value, int tests) {
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++)
        set.insert(value);

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time how long it takes to insert a value at the specified index
 * @param vector Vector to insert value to
 * @param index  Index to insert value to
 * @param value  Value to insert
 * @param tests  Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeInsert(vector<string>& vect, int index, string value, int tests) {
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++)
        vect.insert(vect.begin() + index, value);

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time how long it takes to insert a value at the specified position. 
 * Does not track the time needed to iterate through the list to retrieve the iterator at the proper position.
 * @param l     List to insert value to
 * @param index Index to insert value to
 * @param value Value to insert
 * @param tests Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeInsert(list<string>& l, int index, string value, int tests) {
    //iterate through list to the specified location
    list<string>::iterator location = l.begin();
    for (int i = 0; i < index; i++) { location++; }
    auto start = high_resolution_clock::now();

    for (int i = 0; i < tests; i++) {

        //insert element at the specified location
        
            l.insert(location, value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time how long it takes to delete a value from a set
 * @param set Set to delete a value from
 * @param index Index to delete
 * @param tests Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeDelete(set<string>& testSet, int index, int tests) {
    //iterate through set to the specified location
    set<string>::iterator location = testSet.begin();
        for (int i = 0; i < index; i++) { location++; }
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++) {
        //delete value at specified location
        testSet.erase(location);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time how long it takes to delete a value at the specified index
 * @param vector Vector to delete a value from
 * @param index  Index to delete a value from
 * @param tests  Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeDelete(vector<string>& vect, int index, int tests) {
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++)
        vect.erase(vect.begin() + index);

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}

/**
 * Time how long it takes to delete a value at the specified position. 
 * Does not track the time needed to iterate through the list to retrieve the iterator at the proper position.
 * @param l     List to delete value of
 * @param index Index to delete
 * @param tests Number of times to repeat test
 * @return Duration in microseconds
 */
microseconds TimeDelete(list<string>& l, int index, int tests) {
    //iterate through list to the specified location; does not start timer
    list<string>::iterator location = l.begin();
    for (int i = 0; i < index; i++) { location++; }

    auto start = high_resolution_clock::now();

    for (int i = 0; i < tests; i++) {
        //delete element at the specified location
        l.erase(location);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start);
}