/***********************************
    Dallas Seroski
    Data Analyser
************************************/
#include "DataAnalysis.h"

/*
description: builds an Entry object from a json string
params: 
returns: 
*/

EntryStruct DataAnalyser::buildEntryFromJsonString(string json_str) {
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
	EntryStruct temp;
    temp.name = activeProcess;
    temp.text = text;
    temp.duration = numduration;
    return temp;
}

vector<EntryStruct> DataAnalyser::buildArray(vector<EntryStruct> & v, EntryStruct entry) {
    int i, found = 0;
    vector<EntryStruct> temp;
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

void DataAnalyser::analyse(vector<EntryStruct> & v) {
    string swear[] = {"dumb", "stupid", "donkey"};
    int numswear[] = {0, 0, 0};
    string unprod[] = {"steam", "uplay", "origin", "solitaire", "minecraft", "half-life 2", "portal", "civilization 5", "defcon", "torn", "868-hack", "initium", "minesweeper", "pinball", "dota 2", "League of Legends",
                       "hearthstone", "Heroes Of the storm", "team fortress 2", "pinball fx2", "visual pinball", "future pinball", "iji", "caveman", "rust", "battlegrounds", "facebook", "twitter", "America's Army: Special Forces",
                       "gta 2", "gta 3", "gta 4", "gta 5"};
    ofstream myfile;
    myfile.open("../email_manager/out.txt");
    int totalLog = 0; 
    int hours, minutes, seconds; 
    vector<EntryStruct> temp;
	EntryStruct top[5];
    temp = v;
    int i, j;
	EntryStruct ent;
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
        str = temp[i].text;
        for(j = 0; j < sizeof(swear)/sizeof(swear[0]); j++) {
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
    myfile << setw(2) << setfill('0') << hours << " ";
    myfile << setw(2) << setfill('0') << minutes%60 << " ";
    myfile << setw(2) << setfill('0') << totalLog%60 << "\n";

    int unprodTime = 0;
    int prodTime = 0;
    // Gets unproductive time
    for(i = 0; i < temp.size(); i++) {
        str = temp[i].name;
        for(j = 0; j < sizeof(unprod)/sizeof(unprod[0]); j++) {
            found = str.find(unprod[j]);
            if(found != -1)
                unprodTime += temp[i].duration;
        }
    }
    
    // Sets productive time
    prodTime = totalLog - unprodTime;

    // Converts productive time to H M S and sends it to the file
    minutes = prodTime / 60;
    hours = minutes / 60;
    myfile << setw(2) << setfill('0') << hours << " ";
    myfile << setw(2) << setfill('0') << minutes%60 << " ";
    myfile << setw(2) << setfill('0') << prodTime%60 << "\n";

    // Converts unproductive time to H M S and sends it to the file
    minutes = unprodTime / 60;
    hours = minutes / 60;
    myfile << setw(2) << setfill('0') << hours << " ";
    myfile << setw(2) << setfill('0') << minutes%60 << " ";
    myfile << setw(2) << setfill('0') << unprodTime%60 << "\n";

    // Sets productivity score and prints it to the file
    float score = (float)prodTime / (float)totalLog;
    score = roundf(score * 100) / 100;
    myfile << score << "\n";

    // Top 5 programs output
    int top5Durations = 0;

    // Sets top 5 durations
    for(i = 0; i < 5; i++) {
        top5Durations += top[i].duration;
    }
    
    /*
        Fixes situations where the percentages don't add up to 1.0
        by either adding or subtracting .01 to top5 (the smallest amount)
        However, if top5 is smaller than .02 it will subtract .01 from
        top4 (the next smallest).
    */
    float top1, top2, top3, top4, top5;
    top1 = (float)top[0].duration / (float)top5Durations;
    top1 = roundf(top1 * 100) / 100;
    top2 = (float)top[1].duration / (float)top5Durations;
    top2 = roundf(top2 * 100) / 100;
    top3 = (float)top[2].duration / (float)top5Durations;
    top3 = roundf(top3 * 100) / 100;
    top4 = (float)top[3].duration / (float)top5Durations;
    top4 = roundf(top4 * 100) / 100;
    top5 = (float)top[4].duration / (float)top5Durations;
    top5 = roundf(top5 * 100) / 100;
    while((float)(top1 + top2 + top3 + top4 + top5) > 1.0) {
        if(top5 < .02)
            top4 = top4 - .01;
        else
            top5 = top5 - .01;
    }
    while((float)(top1 + top2 + top3 + top4 + top5) < 1.0) {
        top5 = top5 + .01;
    }

    // Outputs the top 5 programs and percentages
    myfile << top[0].name << "\n" << top1 << "\n";
    myfile << top[1].name << "\n" << top2 << "\n";
    myfile << top[2].name << "\n" << top3 << "\n";
    myfile << top[3].name << "\n" << top4 << "\n";
    myfile << top[4].name << "\n" << top5 << "\n";

    // Prints out swear words and how many times they appear to file
    for(i = 0; i < 3; i++) { 
        if(numswear[i] == 0)
            continue;
        else
            myfile << swear[i] << endl << numswear[i] << "\n";
    }
}

/*
// testing 
int main() {
    string json_str1, json_str2, json_str3, json_str4, json_str5, json_str6;
    string activeProcess, procid, startTime, endTime, duration, text;
	activeProcess = "chrome";
    procid = "2476";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "3292";
    text = "This is a  of wh@t the www.facebook.com keylogg3r will be l0gging.";
    json_str1 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";
    
    activeProcess = "explorer";
    procid = "12392";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "3112";
    text = "This is a  of wh@t the www.facebook.com keyloggging.";
    json_str2 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";
    
    activeProcess = "WINWORD";
    procid = "1232";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "3600";
    text = "Thi wh@t the www.facebook.com stupid will be l0gging.";
    json_str3 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    activeProcess = "solitaire";
    procid = "2422";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "850";
    text = "This is a stupid keylogg3r will be l0gging.";
    json_str4 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    activeProcess = "steam";
    procid = "2472";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "5922";
    text = "This is a stupid of wh@t the www.fal be donkey.";
    json_str5 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    activeProcess = "tes";
    procid = "2476";
    startTime = "03/31/2017-00:06:43";
    endTime = "03/31/2017-00:12:23";
    duration = "10";
    text = "This is a sampl www.facebook.com donkey will be l0gging.";
    json_str6 = "{\"active_process\": \"" + activeProcess + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"},";

    DataAnalyser data;
    vector<EntryStruct> vect;
	EntryStruct ent;
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
    data.analyse(vect);
}
*/
