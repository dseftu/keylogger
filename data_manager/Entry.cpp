#include <string>
using std::string

class Entry {
    // all the values are held as ascii strings
    string startTime, endTime, duration, text, programName;
    public:
        Entry(string, string, string, string, string, string);
        string toString();
};

Entry::Entry (string startTime, string endTime, string duration, 
                string text, string programName) {
    self.startTime = startTime;
    self.endTime = endTime;
    self.duration = duration;
    self.text = text;
    self.programName = programName;
}

Entry::toString() {
    string temp = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"}";
    return temp;
}