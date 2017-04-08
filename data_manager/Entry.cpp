
#include "Entry.h"

Entry::Entry() {
	startTime = "";
	endTime = "";
	duration = "";
	text = "";
	programName = "";
	procid = "";
}

Entry::Entry (string _startTime, string _endTime, string _duration, 
                string _text, string _programName, string _procid) {
    startTime = _startTime;
    endTime = _endTime;
    duration = _duration;
    text = _text;
    programName = _programName;
    procid = _procid;
}

string Entry::toString() {
    string temp = "{\"active_process\": \"" + programName + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"}";
    return temp;
}