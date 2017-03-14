#include <string>
#include <iostream>
#include <fstream>
#include <EncryptionManager.cpp>
#include <DataAnalysis.cpp>

class DataManager {
    public:
        void put(Entry);
        DataManager();
    private:
        void seal();
        void init();
}

DataManager::DataManager() {

}

/*
description: appends an entry to the end of outfile.txt
params: 
returns: 
*/
DataManager::put(entry Entry) {
    ofstream infile;
    infile.open("json_schema.txt", ios::in);

    ofstream outfile;
    outfile.open("outfile.txt", ios::app);
}

/*
description: seals outfile.txt so that no more entries can be added. This is done at the 
    end of the day at 6:00pm.
params: 
returns: 
*/
DataManager::seal() {
    ofstream outfile;
    outfile.open("outfile.txt", ios::app);

    string close_bracket = "}"
}

/*
description: 
params: 
returns: 
*/
DataManager::init() {
    ofstream outfile;
    outfile.open("outfile.txt", ios::app);

    string open_bracket = "{"

}