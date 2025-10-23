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

milliseconds Read(list<string>& list, ifstream& input);
milliseconds Read(set<string>& set, ifstream& input);
milliseconds Read(vector<string>& vect, ifstream& input);

milliseconds TimeSort(vector<string>& vector, int tests);
milliseconds TimeSort(list<string>& list, int tests);

milliseconds TimeInsert(vector<string>& vector, int index, string value, int tests);
milliseconds TimeInsert(set<string>& set, string value, int tests);
milliseconds TimeInsert(list<string>& list, int index, string value, int tests);

milliseconds TimeDelete(vector<string>& vector, int index, int tests);
milliseconds TimeDelete(set<string>& set, int index, int tests);
milliseconds TimeDelete(list<string>& list, int index, int tests);

vector<milliseconds> ReadRace(list<string>& list, vector<string>& vect, set<string>& set, string filename, int tests);
vector<milliseconds> SortRace(list<string>& list, vector<string>& vect, int tests);
vector<milliseconds> InsertRace(list<string>&list, vector<string>& vect, set<string>& set, string value, int tests);
vector<milliseconds> DeleteRace(list<string>&list, vector<string>& vect, set<string>& set, int tests);

void OutputRace(vector<milliseconds> durations, string raceName, int spacing = DEFAULT_SPACING);
void OutputRace(vector<string> names, int spacing = DEFAULT_SPACING);

int main() {
    const string FILENAME = "codes.txt";
    const int TESTS = 100;
    list<string> list;
    set<string> set;
    vector<string> vect;
    
    OutputRace(vector<string>{"List", "Vector", "Set"});
    OutputRace(ReadRace(list, vect, set, FILENAME, 10), "Read");
    OutputRace(SortRace(list, vect, TESTS), "Sort");
    OutputRace(InsertRace(list, vect, set, "TESTCODE", TESTS), "Insert");
    OutputRace(DeleteRace(list, vect, set, TESTS), "Delete");

    return 0;
}

/* syntax examples:
auto start = high_resolution_clock::now()
auto end = high_resolution_clock::now()
auto duration = duration_cast<milliseconds>(end - start)
duration.count() references elapsed milliseconds
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
void OutputRace(vector<milliseconds> durations, string raceName, int spacing) {
    //output race name
    cout << left << setw(spacing) << raceName;

    //followed by entries
    for (milliseconds duration : durations) {
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
vector<milliseconds> DeleteRace(list<string>&testList, vector<string>& vect, set<string>& set, int tests) {
    //Run timers and return as an array
    return vector<milliseconds>
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
vector<milliseconds> InsertRace(list<string>&testList, vector<string>& vect, set<string>& set, string value, int tests) {
    //Run timers and return as an array
    return vector<milliseconds>
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
vector<milliseconds> SortRace(list<string>& list, vector<string>& vect, int tests) {
    //Run timers and return as an array. The third parameter is set to negative one for a set
    return vector<milliseconds>{TimeSort(list, tests), TimeSort(vect, tests), static_cast<milliseconds>(-1)};
}

/**
 * Run race for read operations on given list, vector, and set
 * Returns vector of durations, ordered [0] list, [1] vector, [2] set
 * @param list List to read to
 * @param set Set to read to
 * @param vector Vector to read to
 * @param filename File to read data from
 * @return Vector of durations in milliseconds, ordered list, vector, set
 */
vector<milliseconds> ReadRace(list<string>& testList, vector<string>& testVector, set<string>& testSet, string filename, int tests) {
    vector<milliseconds> durations;
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
        durations.push_back(Read((i == 0 ? testList : dummyList), infile)); 

        //Reset file stream to beginning
        infile.clear();
        infile.seekg(0);

        //Time read operation for vector
        durations.push_back(Read((i == 0) ? testVector : dummyVect, infile));
        infile.clear();
        infile.seekg(0);
        
        //Time read operation for set
        durations.push_back(Read((i == 0) ? testSet : dummySet, infile));
        infile.clear();
        infile.seekg(0);
    }

    infile.close();

    return durations;
}

/**
 * Time how long it takes to read data into a list
 * @param list List to read data to
 * @param input Input stream to read from
 * @return Duration in milliseconds 
 */
milliseconds Read(list<string>& list, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to list
    string value;
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
milliseconds Read(set<string>& set, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to set
    string value;
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
milliseconds Read(vector<string>& vector, ifstream& input) {
    auto start = high_resolution_clock::now();

    //Read all data from input stream and add to vector
    string value;
    while (getline(input, value)) {
        vector.push_back(value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time sort function of a list
 * @param list  List to sort
 * @param tests Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeSort(list<string>& l, int tests) {
    milliseconds totalTime(0);
    for (int i = 0; i < tests; i++) {
        list<string> testList = l;

        auto start = high_resolution_clock::now();
        testList.sort();
        auto end = high_resolution_clock::now();

        totalTime += duration_cast<milliseconds>(end - start);
    }
    return totalTime;
}

/**
 * Time sort function of a vector
 * @param vector Vector to sort
 * @param tests  Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeSort(vector<string>& vect, int tests) {
    milliseconds totalTime(0);
    for (int i = 0; i < tests; i++) {
        vector<string> testVect = vect;

        auto start = high_resolution_clock::now();
        sort(testVect.begin(), testVect.end());
        auto end = high_resolution_clock::now();

        totalTime += duration_cast<milliseconds>(end - start);
    }
    return totalTime;
}

/**
 * Time how long it takes to add a value to a set
 * @param set   Set to insert a value to
 * @param value Value to insert into set
 * @param tests Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeInsert(set<string>& set, string value, int tests) {
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++)
        set.insert(value);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to insert a value at the specified index
 * @param vector Vector to insert value to
 * @param index  Index to insert value to
 * @param value  Value to insert
 * @param tests  Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeInsert(vector<string>& vect, int index, string value, int tests) {
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++)
        vect.insert(vect.begin() + index, value);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to insert a value at the specified position. 
 * Tracks the time needed to iterate through the list to retrieve the iterator at the proper position.
 * @param l     List to insert value to
 * @param index Index to insert value to
 * @param value Value to insert
 * @param tests Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeInsert(list<string>& l, int index, string value, int tests) {
    auto start = high_resolution_clock::now();

    for (int i = 0; i < tests; i++) {
        //iterate through list to the specified location
        list<string>::iterator location = l.begin();
        for (int i = 0; i < index; i++) { location++; }

        //insert element at the specified location
        
            l.insert(location, value);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to delete a value from a set
 * @param set Set to delete a value from
 * @param index Index to delete
 * @param tests Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeDelete(set<string>& testSet, int index, int tests) {
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++) {
        //iterate through list to the specified location
        set<string>::iterator location = testSet.begin();
            for (int i = 0; i < index; i++) { location++; }
        //delete value at specified location
        testSet.erase(location);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to delete a value at the specified index
 * @param vector Vector to delete a value from
 * @param index  Index to delete a value from
 * @param tests  Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeDelete(vector<string>& vect, int index, int tests) {
    auto start = high_resolution_clock::now();

    for(int i = 0; i < tests; i++)
        vect.erase(vect.begin() + index);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to delete a value at the specified position. 
 * Tracks the time needed to iterate through the list to retrieve the iterator at the proper position.
 * @param l     List to delete value of
 * @param index Index to delete
 * @param tests Number of times to repeat test
 * @return Duration in milliseconds
 */
milliseconds TimeDelete(list<string>& l, int index, int tests) {
    auto start = high_resolution_clock::now();

    for (int i = 0; i < tests; i++) {
        //iterate through list to the specified location
        list<string>::iterator location = l.begin();
        for (int i = 0; i < index; i++) { location++; }

        //delete element at the specified location
        l.erase(location);
    }

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}