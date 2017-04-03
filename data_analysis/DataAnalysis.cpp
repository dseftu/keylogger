//#include <DataManager.cpp>
//#include <Entry.cpp>
#include <string>
#include <sstream>
#include <algorithm>

using std::string;
using namespace std;

class DataAnalyser {
    public:
        buildEntryFromJsonString(string json_str);
    
};

/*
description: builds an Entry object from a json string
params: 
returns: 
*/
// TOD
DataAnalyser::buildEntryFromJsonString(string json_str) {
    string arr[5000];
    string activeProcess = "", duration, text = "";
    int halt;
    char ch;
    int i = 0, j = 0;
    /*
        This splits json_str into a stringstream and sends that into an array
        split over spaces
    */
    stringstream ssin(json_str);
    while(ssin.good() && i < 500) {
        ssin >> arr[i];
        i++;
    }
    /*
        This cleans up array of strings to put all in lowercase
        and remove '{', '"', ',', and ':' from the beginning and end of each string
        After this it is ready to be thrown into an entry object
    */
    for(j = 0; j < i; j++) {
       ch = *arr[j].begin();
       // removes { from beginning
       if(ch == '{') {
           arr[j].erase(arr[j].begin());
           ch = *arr[j].begin();
       } 
       // removes " from begining
       if(ch == '"') 
            arr[j].erase(arr[j].begin());
        ch = *arr[j].rbegin();
        // removes , from end
        if(ch == ',') {
            arr[j].erase(arr[j].size()-1);
            ch = *arr[j].rbegin();
        }
        // removes : from end
        if(ch == ':') {
            arr[j].erase(arr[j].size()-1);
            ch = *arr[j].rbegin();
        }
        // removes } from end
        if(ch == '}') {
            arr[j].erase(arr[j].size()-1);
            ch = *arr[j].rbegin();
        }
        // removes " from end
        if(ch == '"')
            arr[j].erase(arr[j].size()-1);

        // Turns all uppcase characters in the strings to lowercase
        transform(arr[j].begin(), arr[j].end(), arr[j].begin(), ::tolower);
    }

    // Gets Active Process
    for(j = 0; j < i; j++) {
        if(arr[j] == "active_process") {
            do {
                j++;
                activeProcess += arr[j] + " ";
            } while(arr[j+1] != "process_id" && j < i);
        }
    }
    // Gets Duration
    for(j = 0; j < i; j++) {
        if(arr[j] == "session_duration")
            duration = arr[j+1];
    }

    // Gets logged keystrokes
    for(j = 0; j < i; j++) {
        if(arr[j] == "logged_keystrokes") {
            do {
                j++;
                text += arr[j] + " ";
            } while(j < i);
        }
    }
    /*
    cout << activeProcess << endl;
    cout << duration << endl;
    cout << text << endl;
    */
    
}

// testing 
int main() {
    string json_str;
    string activeProcess, procid, startTime, endTime, duration, text;
	activeProcess = "chrome test again";
    procid = "2476";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "340";
    text = "This is a sample of wh@t the keylogg3r will be l0gging.";
    json_str = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";
    DataAnalyser data;
    data.buildEntryFromJsonString(json_str);
}