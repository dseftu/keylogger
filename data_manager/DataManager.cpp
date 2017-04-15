// Eric
#include "DataManager.h"
#include <vector>

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
	entryString += "                                                             ";
	

	int slen = entryString.length();
	int len = slen + (16 - (slen % 16));
	unsigned char* ciphertext = new unsigned char[len];
	em.encrypt(entryString, ciphertext, len);
	
	outfile.write((char*)(&ciphertext[0]), len);
	outfile.flush();	
	outfile.close();
}

/*
description: gets all of the day's output. This is done at the end of the day at 6:00pm.
params: none
returns: none
*/
void DataManager::DumpDay() {

	//open file with cursor at end. find file length. close file.
	vector<unsigned char> bytes;
	ifstream infile("outfile.bin", ios::in | ios::binary | ios::ate); 
	
	infile.seekg(0, ios::beg);
	unsigned char ch = infile.get();
	while (infile.good())
	{
		bytes.push_back(ch);
		ch = infile.get();
	}
	size_t size = bytes.size();
	string line = em.decrypt(bytes);

	infile.close();	

	char * plaintext = new char[size + 1];
	std::copy(line.begin(), line.end(), plaintext);
	plaintext[size] = '\0'; 

	DataAnalyser da;
	vector<EntryStruct> vect;
	EntryStruct ent;

	
	char curly = '}';
	string outJson;
	int count = 0;

	for (int i = 0; i < size; i++)
	{
		
		stringstream ss;
		ss << plaintext[i];
		string temp2 = ss.str();

		outJson = outJson + temp2;
		
		if (plaintext[i] == curly)
		{
			count++;
			
			//send Json string to DataAnalysis		
			ent = da.buildEntryFromJsonString(outJson);
			vect = da.buildArray(vect, ent);

			outJson.clear();
		}
	}
	da.analyse(vect);



	/*
   
    string out_json;
    int len;
    
	unsigned char* ciphertext = new unsigned char[BUFFER_LENGTH];
    vector<EntryStruct> entryVector;
    if(infile.is_open()) {
        size_t file_length = infile.tellg();
		infile.seekg(0, ios::beg);

        // main loop, goes through the file decrypting 16 bytes at a time.
        for(size_t i = 0; i < file_length; i += BUFFER_LENGTH){
			cout << "reading chars from file" << endl;
			infile.read((char*)ciphertext, BUFFER_LENGTH);
            //line += em.decrypt(ciphertext, BUFFER_LENGTH);
			cout << line << endl;
            // looks to see if there is a opening and closing curly brace {}
            len = line.length();
			cout << "Line Length: " << len << endl;
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
	*/

	//cout << "Successfully dumped days information" << endl;
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


/*
int main()
{

	EmailManager em;
	em.readAnalysisResults();

	return 0;
}
*/
