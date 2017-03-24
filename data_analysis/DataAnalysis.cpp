#include <DataManager.cpp>
#include <Entry.cpp>
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
    string arr[500];
    char ch;
    int i = 0, j = 0;
    stringstream ssin(json_str);
    while(ssin.good() && i < 500) {
        ssin >> arr[i];
        i++;
    }
    /*
        This cleans up array of strings to put all in lowercase
        and remove '{', '"', ',' from the beginning and end of each string
        After this it is ready to be thrown into an entry object
    */
    //TODO
    for(j = 0; j < i; j++) {
       ch = *arr[j].begin();
       if(ch == '{') {
           arr[j].erase(arr[j].begin());
           ch = *arr[j].begin();
       } 
       if(ch == '"') 
            arr[j].erase(arr[j].begin());
        ch = *arr[j].rbegin();
        if(ch == ',') {
            arr[j].erase(arr[j].size()-1);
            ch = *arr[j].rbegin();
        }
        if(ch == '}') {
            arr[j].erase(arr[j].size()-1);
            ch = *arr[j].rbegin();
        }
        if(ch == '"')
            arr[j].erase(arr[j].size()-1);
        transform(arr[j].begin(), arr[j].end(), arr[j].begin(), ::tolower);
    }
}