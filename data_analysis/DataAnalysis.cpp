//#include <DataManager.cpp>
//#include "..\data_manager\Entry.cpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

using std::string;
using std::vector;
using namespace std;

struct Entry {
    string name;
    string text;
    int duration;
};


class DataAnalyser {
    public:
        Entry buildEntryFromJsonString(string json_str);
        vector<Entry> buildArray(vector<Entry> & v, Entry entry);
        analyse(vector<Entry> & v);
        
};

/*
description: builds an Entry object from a json string
params: 
returns: 
*/
// TOD
Entry DataAnalyser::buildEntryFromJsonString(string json_str) {
    string arr[5000];
    string activeProcess = "", duration, text = "";
    int numduration; 
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
    // Converts duration strng to an integer
    numduration = atoi(duration.c_str());

    // Builds Entry to return
    Entry temp;
    temp.name = activeProcess;
    temp.text = text;
    temp.duration = numduration;
    return temp;
}

vector<Entry> DataAnalyser::buildArray(vector<Entry> & v, Entry entry) {
    int i, found = 0;
    vector<Entry> temp;
    temp = v;
    // If name is in the vector, increase duration by new entry duration
    // else add it to the vector
    for(i = 0; i < temp.size(); i++) {
        if(temp[i].name == entry.name) {
            temp[i].duration += entry.duration;
            return temp;
        }
    }
    temp.push_back(entry);
    return temp;
}

DataAnalyser::analyse(vector<Entry> & v) {
    vector<Entry> temp;
    Entry top[5];
    temp = v;
    int i, j;
    for(i = 0; i < 5; i++) {
        
    }

}


// testing 
int main() {
    string json_str;
    string activeProcess, procid, startTime, endTime, duration, text;
	activeProcess = "chrome.exe";
    procid = "2476";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "340";
    text = "This is a sample of wh@t the www.facebook.com keylogg3r will be l0gging.";
    json_str = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";
    DataAnalyser data;
    vector<Entry> vect; 
    Entry ent = data.buildEntryFromJsonString(json_str);
    Entry ent2 = data.buildEntryFromJsonString(json_str);
    vect = data.buildArray(vect, ent);
    int i;
    for(i = 0; i < vect.size(); i++) {
        cout << vect[i].name << endl;
        cout << vect[i].duration << endl;
        cout << vect[i].text << endl;
    }
    cout << endl;
    vect = data.buildArray(vect, ent2);
    for(i = 0; i < vect.size(); i++) {
        cout << vect[i].name << endl;
        cout << vect[i].duration << endl;
        cout << vect[i].text << endl;
    }
    cout << endl;
    ent2.name = "explorer.exe";
    vect = data.buildArray(vect, ent2);
    for(i = 0; i < vect.size(); i++) {
        cout << vect[i].name << endl;
        cout << vect[i].duration << endl;
        cout << vect[i].text << endl;
    }
    cout << endl << endl << endl;
}
