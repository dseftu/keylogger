/***********************************
    Dallas Seroski
    Data Analyser
************************************/

#include "..\data_manager\DataManager.h"
#include "..\data_manager\Entry.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <cmath>
using std::string;
using std::vector;
using namespace std;

struct Entry {
    string name;
    string text;
    int duration;
};

struct myclass {
    bool operator() (Entry i, Entry j) { return (j.duration < i.duration);}
} comp;

class DataAnalyser {
    public:
        Entry buildEntryFromJsonString(string json_str);
        vector<Entry> buildArray(vector<Entry> & v, Entry entry);
        analyse(vector<Entry> & v);
        
};
