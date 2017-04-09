#pragma once
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <encryption_manager.h>

#include "..\data_analysis\DataAnalysis.h"
#include "..\data_manager\Entry.h"
#include "rapidjson\document.h"


using std::string;
using std::vector;

#define BUFFER_LENGTH 16



using namespace rapidjson;

class DataManager {
    public:
        void put(Entry);
        DataManager();
		void DumpDay();
    private:        
        void init();
};

