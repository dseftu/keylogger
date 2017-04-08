#include <string>
#include <iostream>
#include <fstream>

#include <encryption_manager.h>
#include "..\data_analysis\DataAnalysis.h"
#include "..\data_manager\Entry.h"
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
};


DataManager::DataManager() {
}