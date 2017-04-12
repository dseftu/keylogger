/***********************************
    Dallas Seroski
    Data Analyser
************************************/
#pragma once
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

class DataAnalyser {
    public:
		EntryStruct buildEntryFromJsonString(string json_str);
        vector<EntryStruct> buildArray(vector<EntryStruct> & v, EntryStruct entry);
        void analyse(vector<EntryStruct> & v);        
};

static struct myclass {
	bool operator() (EntryStruct i, EntryStruct j) { return (j.duration < i.duration); }
} comp;