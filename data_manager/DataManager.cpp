#include <string>
#include <iostream>
#include <fstream>
#include <EncryptionManager.cpp>
#include <DataAnalysis.cpp>
#include <Entry.cpp>
#include <cstring>
#include <rapidjson/document.h>

using namespace rapidjson;

class DataManager {
    public:
        void put(Entry);
        DataManager();
    private:
        void DumpDay();
        void init();
}

EncryptionManager em;

DataManager::DataManager() {
}

/*
description: appends an entry to the end of outfile.txt
params: 
returns: 
*/
DataManager::put(Entry entry) {
    ofstream infile;
    infile.open("json_schema.txt", ios::in);

    ofstream outfile;
    outfile.open("outfile.txt", ios::app);
    string entryString = entry.toString();
    
    int len = entryString.length();
    unsigned char* ciphertext[len];
    em.encrypt(entryString, ciphertext, len);
    
    // outfile.append(entry.toString());
}

/*
description: seals the day's outfile.txt so that no more entries can be added. This is done at the 
    end of the day at 6:00pm.
params: 
returns: 
*/
DataManager::DumpDay() {
    ofstream outfile;
    // outfile.open("outfile.txt", ios::app);
    string line;
    string out_json;
    ifstream outfile ("outfile.txt");
    if(outfile.is_open()) {
        while(getline (outfile,line)) {
            cout << line; // DEBUG
        }
        outfile.close();
    }
    else cout << "Unable to open file";

}

/*
description: begins the new outfile
params: 
returns: 
*/
DataManager::init() {
    ofstream outfile;
    outfile.open("outfile.txt", ios::trunc);
    outfile << "hello world";
}
// test
int main() {
    cout < "Hello World!";
}