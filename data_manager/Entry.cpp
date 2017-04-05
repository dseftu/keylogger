#include <string>
using std::string

class Entry {
    // all the values are held as ascii strings
    string entryType, endTime, text, encoding, keyCombo, programName, activeWindow, startTime;
    public:
        Entry(string);
        Entry(string, string, string, string, string, string);
        string toString();
};

Entry::Entry (string startTime) {
    self.startTime = startTime;
    self.entryType = "";
    self.endTime = "";
    self.text = "";
    self.encoding = "";
    self.keyCombo = "";
    self.programName = "";
    self.activeWindow = "";
}

Entry::Entry (string entryType, string startTime, string endTime, 
                string text, string encoding, string keyCombo, 
                string programName, string activeWindow) {
    self.entryType = entryType;
    self.startTime = startTime;
    self.endTime = endTime;
    self.text = text;
    self.encoding = encoding;
    self.keyCombo = keyCombo;
    self.programName = programName;
    self.activeWindow = activeWindow;
}

Entry::toString() {
    string temp = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"}";
    // string temp = "{\"entryType\" : \"" + entryType + "\", \"startTime\" : \"" + startTime + "\", \"endTime\" : \"" + endTime + "\", \"text\" : \"" + text + "\", \"encoding\" : \"" + encoding + "\", \"keyCombo\" : \"" + keyCombo + "\", \"program\" : \"" + programName + "\", \"activeWindow\" : \"" + activeWindow + "\"},";
    return temp;
}