#pragma once
#include <iostream>
#include <fstream>
#include <conio.h>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include "curl\curl.h"
#include "libharu\hpdf.h"

using namespace std;

#pragma warning(disable: 4996)
#pragma warning(disable: 4305)
#define FROM    "<dankestlogger@gmail.com>"    //need password for from
//#define TO      "<TheManager117@gmail.com>" //don't need password for to
#define FILENAME "report.pdf"

struct fileBuf_upload_status
{
	int lines_read;
};

//Globals
static const int CHARS = 76;     //Sending 54 chararcters at a time with \r , \n and \0 it becomes 57
static const int ADD_SIZE = 7;   // ADD_SIZE for TO,FROM,SUBJECT,CONTENT-TYPE,CONTENT-TRANSFER-ENCODING,CONETNT-DISPOSITION and \r\n
static const int SEND_BUF_SIZE = 54;
static char(*fileBuf)[CHARS] = NULL;
static const char cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
string theDate, employee, email;
string totalLT, prodLT, unprodLT, prodScore;
string prog1, prog2, prog3, prog4, prog5;
HPDF_REAL p1, p2, p3, p4, p5;
string swear[200], swearFreq[200];
int numSwears=0;
char* TO;

class EmailManager
{
	public:
		void readAnalysisResults();
		void createPDF();
		void emailPDF();	

};

void error_handler(HPDF_STATUS, HPDF_STATUS, void*);
void encodeblock(unsigned char[3], unsigned char[4], int);
void encode(FILE, unsigned char *, int);
size_t read_file();
static size_t fileBuf_source(void*, size_t, size_t, void *);