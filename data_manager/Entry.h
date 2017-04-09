#pragma once
#include <string>
using std::string;

class Entry {
    // all the values are held as ascii strings
    
    public:
		string startTime, endTime, duration, text, programName, procid;
		Entry();
        Entry(string, string, string, string, string, string);
        string toString();
};

struct EntryStruct {
	string name;
	string text;
	int duration;
};

