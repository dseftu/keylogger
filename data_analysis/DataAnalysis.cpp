//#include <DataManager.cpp>
//#include "..\data_manager\Entry.cpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

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
    string swear[] = {"dumb", "stupid", "donkey"};
    int numswear[] = {0, 0, 0};

    ofstream myfile;
    myfile.open("out.txt");
    int totalLog = 0; 
    int hours, minutes, seconds; 
    vector<Entry> temp;
    Entry top[5];
    temp = v;
    int i, j;
    Entry ent;
    ent.name = "null";
    ent.duration = 0;
    ent.text = "null";

    // Sorts vector based on duration
    sort(temp.begin(), temp.end(), comp);
    for(i = 0; i < 5; i++) {
        top[i] = temp[i];
    }

    // Counts swear words
    string str;
    int found;
    for(i = 0; i < temp.size(); i++) {
        for(j = 0; j < 3; j++) {
            str = temp[i].text;
            found = str.find(swear[j]);
            if(found > 0) numswear[j]++;
        }
    }

    // Adds up total Log Time
    for(i = 0; i < temp.size(); i++)
        totalLog += temp[i].duration;
        
    // Converts totalLog Time to H M S and sends it to file
    minutes = totalLog / 60;
    hours = minutes / 60;
    myfile << hours << " " << minutes%60 << " " << totalLog%60 << "\n";



    // Prints out swear words and how many times they appear to file
    for(i = 0; i < 3; i++) { 
        if(numswear[i] == 0)
            continue;
        else
            myfile << swear[i] << endl << numswear[i] << "\n";
    }
    
    
}

/*
dummy
donkey
stupid
facebook
myspace
*/

// testing 
int main() {
    string json_str1, json_str2, json_str3, json_str4, json_str5, json_str6;
    string activeProcess, procid, startTime, endTime, duration, text;
	activeProcess = "chrome.exe";
    procid = "2476";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "340";
    text = "This is a  of wh@t the www.facebook.com keylogg3r will be l0gging.";
    json_str1 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";
    
    activeProcess = "explorer.exe";
    procid = "2477";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "350";
    text = "This is a  of wh@t the www.facebook.com keyloggging.";
    json_str2 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";
    
    activeProcess = "WINWORD.exe";
    procid = "2478";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "3600";
    text = "Thi wh@t the www.facebook.com stupid will be l0gging.";
    json_str3 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    activeProcess = "crazy.exe";
    procid = "2422";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "850";
    text = "This is a stupid keylogg3r will be l0gging.";
    json_str4 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    activeProcess = "crazierrrr.exe";
    procid = "2472";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "900";
    text = "This is a stupid of wh@t the www.fal be donkey.";
    json_str5 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    activeProcess = "tes.exe";
    procid = "2476";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "10";
    text = "This is a sampl www.facebook.com donkey will be l0gging.";
    json_str6 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    DataAnalyser data;
    vector<Entry> vect; 
    Entry ent;
    ent = data.buildEntryFromJsonString(json_str1);
    vect = data.buildArray(vect, ent);
    ent = data.buildEntryFromJsonString(json_str2);
    vect = data.buildArray(vect, ent);
    ent = data.buildEntryFromJsonString(json_str3);
    vect = data.buildArray(vect, ent);
    ent = data.buildEntryFromJsonString(json_str4);
    vect = data.buildArray(vect, ent);
    ent = data.buildEntryFromJsonString(json_str5);
    vect = data.buildArray(vect, ent);
    ent = data.buildEntryFromJsonString(json_str6);
    vect = data.buildArray(vect, ent);
    int i;
    for(i = 0; i < vect.size(); i++) {
        cout << vect[i].name << endl;
        cout << vect[i].duration << endl;
        cout << vect[i].text << endl;
    }
    cout << endl;
    data.analyse(vect);
}