#include <Entry.cpp>
#include <DataManager.cpp>
#include <DataAnalysis.cpp>
#include <EmailManager.cpp>
#include <string>
#include <queue>
using std::queue;

class LoggingManager {
    // there should be an independent thread offloading Entry objects to DataManager
    queue<Entry> *outputBuffer;
    public:
        LoggingManager();
}

LoggingManager::LoggingManager() {
    self.outputBuffer = new queue<Entry>;
}

/*
description: 
params: 
returns: 
*/
int main() {

    LoggingManager loggingManager;

    // start threads here

    while(true) {
        /*
        1. Look for logging events
        2. when a logging event is detected start building a new Entry object
        3. When the logging event is completed, finish any necessary building of the Entry object 
            and push it to the outputBuffer queue.
        */
    }
}