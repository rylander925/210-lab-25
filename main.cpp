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

milliseconds Read(list<string>& list, ifstream& input);
milliseconds Read(set<string>& set, ifstream& input);
milliseconds Read(vector<string>& vect, ifstream& input);

milliseconds TimeSort(vector<string>& vector);
milliseconds TimeSort(list<string>& list);

milliseconds TimeInsert(vector<string>& vector, string value);
milliseconds TimeInsert(set<string>& set, string value);
milliseconds TimeInsert(list<string>& list, string value);

vector<milliseconds> ReadRace(list<string>& list, vector<string>& vect, set<string>& set, string filename);
vector<milliseconds> SortRace(list<string>& list, vector<string>& vect);
vector<milliseconds> InsertRace(list<string>&list, vector<string>& vect, set<string>& set, string value);

void OutputRace(vector<milliseconds> durations, string raceName, int spacing = 5);
void OutputRace(vector<string> names, int spacing = 5);

int main() {
    const string FILENAME = "codes.txt";
    list<string> list;
    set<string> set;
    vector<string> vect;
    vector<milliseconds> readDurations, sortDurations, insertDurations;
    readDurations = ReadRace(list, vect, set, FILENAME);

    cout << list.front() << ", " << *(set.begin()) << ", " << vect.front() << endl;
    for (milliseconds time : readDurations) { cout << time.count() << " "; }
    cout << endl; 

    auto start = high_resolution_clock::now();

    list.sort();

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start);

    cout << TimeSort(list).count() << " " << duration.count();


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
 * Returns durations of insertion operations for a list, vector, and a set
 * @param testList List to insert to
 * @param vect     Vector to insert to
 * @param set      Set to insert to
 * @param value    Value to insert
 * @return Vector containing durations of tests, ordered list, vector, set
 */
vector<milliseconds> InsertRace(list<string>&testList, vector<string>& vect, set<string>& set, string value) {
    //Iterate to middle of list to test insert only
    list<string>::iterator listPos = testList.begin();
    for(int i = 1; i < testList.size() / 2; i++) { listPos++; }

    //Run timers and return as an array
    return vector<milliseconds>
        {   TimeInsert(testList, listPos, value),
            TimeInsert(vect, (vect.begin() + vect.size() / 2), value),
            TimeInsert(set, value)
        };
}

/**
 * Returns resulting durations from sorting race between a list and vector. Third parameter (for a set) is -1.
 * @param list List to sort
 * @param vect Vector to sort
 * @return Vector of sorting durations, ordered list, vector, set
 */
vector<milliseconds> SortRace(list<string>& list, vector<string>& vect) {
    //Run timers and return as an array. The third parameter is set to negative one for a set
    return vector<milliseconds>{TimeSort(list), TimeSort(vect), static_cast<milliseconds>(-1)};
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
vector<milliseconds> ReadRace(list<string>& list, vector<string>& vect, set<string>& set, string filename) {
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

    //Time read operation for vector
    durations.push_back(Read(vect, infile));
    infile.clear();
    infile.seekg(0);
    
    //Time read operation for set
    durations.push_back(Read(set, infile));

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
 * @param list List to sort
 * @return Duration in milliseconds
 */
milliseconds TimeSort(list<string>& list) {
    auto start = high_resolution_clock::now();

    list.sort();

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time sort function of a vector
 * @param vector Vector to sort
 * @return Duration in milliseconds
 */
milliseconds TimeSort(vector<string>& vector) {
    auto start = high_resolution_clock::now();

    sort(vector.begin(), vector.end());

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to add a value to a set
 * @param set Set to insert a value to
 * @param value Value to insert into set
 * @return Duration in milliseconds
 */
milliseconds TimeInsert(set<string>& set, string value) {
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
milliseconds TimeInsert(vector<string>& vect, vector<string>::iterator& location, string value) {
    auto start = high_resolution_clock::now();

    vect.insert(location, value);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}

/**
 * Time how long it takes to insert a value at the specified location
 * @param l List to insert value to
 * @param location Iterator to insert value to
 * @param value Value to insert
 * @return Duration in milliseconds
 * @todo may want to change iterator to an index, and time the iteration duration as well
 */
milliseconds TimeInsert(list<string>& l, list<string>::iterator& location, string value) {
    auto start = high_resolution_clock::now();

    l.insert(location, value);

    auto end = high_resolution_clock::now();
    return duration_cast<milliseconds>(end - start);
}