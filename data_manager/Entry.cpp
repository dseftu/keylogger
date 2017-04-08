#include "Entry.h"

Entry::Entry (string startTime, string endTime, string duration, 
                string text, string programName, string procid) {
    self.startTime = startTime;
    self.endTime = endTime;
    self.duration = duration;
    self.text = text;
    self.programName = programName;
    self.procid = procid;
}

Entry::toString() {
    string temp = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"}";
    return temp;
}