// Eric
#include "DataManager.h"

EncryptionManager em;


DataManager::DataManager() {
    em = EncryptionManager();

}

/*
description: appends an encrypted entry to the end of outfile.bin
params: 
returns: 
*/

void DataManager::put(Entry entry) {

	ofstream outfile;
	
    outfile.open("outfile.bin", ios::app | ios::binary);
    string entryString = entry.toString();
    
    int len = entryString.length();
	unsigned char* ciphertext = new unsigned char[len+16];
    // ciphertext = entryString;
    em.encrypt(entryString, ciphertext, len);
	outfile.write((const char*)&ciphertext[0], len);
	outfile.flush();
	outfile.close();
}

/*
description: gets all of the day's output. This is done at the end of the day at 6:00pm.
params: none
returns: none
*/
void DataManager::DumpDay() {
    ifstream infile("outfile.bin", ios::binary | ios::ate);
	DataAnalyser da;
	
    // outfile.open("outfile.bin", ios::app);
    string line;
    string out_json;
    int len;
    
	unsigned char* ciphertext = new unsigned char[BUFFER_LENGTH];
    vector<EntryStruct> entryVector;
    if(infile.is_open()) {
        size_t file_length = infile.tellg();
		infile.seekg(0, ios::beg);

        // main loop, goes through the file decrypting 16 bytes at a time.
        for(size_t i = 0; i < file_length; i += BUFFER_LENGTH){
			infile.read((char*)ciphertext, BUFFER_LENGTH);
            line += em.decrypt(ciphertext, BUFFER_LENGTH);;
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
                    EntryStruct entry = da.buildEntryFromJsonString(out_json);
					entryVector = da.buildArray(entryVector, entry);
                }
            }
        }
		infile.close();
    }
    else {
        cout << "Unable to open file";
    }

    // finally DataAnalysis analyses all the data.
    da.analyse(entryVector);
    cout << "Successfully dumped days information";
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
