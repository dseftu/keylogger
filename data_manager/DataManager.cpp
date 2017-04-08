#include "DataManager.h"
EncryptionManager em;

#include <string>
#include <iostream>
#include <fstream>
#include "../encryption_manager/EncryptionManager.cpp"
#include "../data_analysis/DataAnalysis.cpp"
#include "Entry.cpp"
#include <cstring>
#include <vector>
using std::string;
using std::vector;

#define BUFFER_LENGTH 16

struct EntryStruct {
    string name;
    string text;
    int duration;
};

class DataManager {
    public:
        bool put(Entry);
        DataManager();
    private:
        bool DumpDay();
        void init();
};

DataManager::DataManager() {
    em = EncryptionManager();
}

/*
description: appends an encrypted entry to the end of outfile.bin
params: 
returns: 
*/

DataManager::put(Entry entry) {
    ifstream outfile;
    outfile.open("outfile.bin", ios::app | ios::binary);
    string entryString = entry.toString();
    
    int len = entryString.length();
    unsigned char* ciphertext[len];
    // ciphertext = entryString;
    em.encrypt(entryString, ciphertext, len);
    outfile.write(ciphertext, len);

    return true;
}

/*
description: gets all of the day's output. This is done at the end of the day at 6:00pm.
params: none
returns: none
*/
void DataManager::DumpDay() {
    ofstream outfile;
    // outfile.open("outfile.bin", ios::app);
    string line;
    string out_json;
    int len;
    ifstream outfile("outfile.bin", ios::binary | ios::ate);
    unsigned char* ciphertext[BUFFER_LENGTH];
    vector<Entry> entryVector;
    if(outfile.is_open()) {
        size_t file_length = outfile.tellg();
        outfile.seekg(0, ios::beg);

        // main loop, goes through the file decrypting 16 bytes at a time.
        for(size_t i = 0; i < file_length; i += BUFFER_LENGTH){
            read(ciphertext, BUFFER_LENGTH);
            em.decrypt(ciphertext, BUFFER_LENGTH);
            line += ciphertext;
            // looks to see if there is a opening and closing curly brace {}
            len = line.length();
            for(size_t j = 0; j < len; j++) {
                bool open_flag = false;
                size_t open_pos;
                if(line.at(j) == '{') {
                    open_flag = true;
                    open_pos = j;
                }

                if(line.at(j) == '}' && open_flag == true) {
                    open_flag = false;
                    out_json = line.substr(open_pos, j - open_pos);
                    line = line.substr(j);
                    cout << "Stripped out json string:" << '\n'; // DEBUG
                    cout << out_json << '\n'; // DEBUG
                    
                    // send the line to DataAnalysis
                    EntryStruct entry = DataAnalysis.buildEntryFromJsonString(out_json);             
                    entryVector = DataAnalysis.buildArray(entryVector, entry)   
                }
            }
        }
        outfile.close();
    }
    else {
        cout << "Unable to open file";
        return false;
    }

    // finally DataAnalysis analyses all the data.
    DataAnalysis.analyse(entryVector);
    cout << "Successfully dumped days information";
    return true;
}

/*
description: deletes and reinitializes outfile, only do this if you're sure you have the days' data
params: none
returns: none
*/
void DataManager::init() {
    ofstream outfile;
    outfile.open("outfile.bin", ios::out | ios::trunc);
    outfile.close();
}
// test
int main() {
    cout << "Hello World!";
}