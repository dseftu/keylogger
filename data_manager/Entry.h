#include <string>
using std::string;

class Entry {
    // all the values are held as ascii strings
    string startTime, endTime, duration, text, programName;
    public:
        Entry(string, string, string, string, string);
        string toString();
};

