
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
int numSwears = 0;
char* TO;


class EmailManager
{
public:
	void readAnalysisResults();
	void createPDF();
	void emailPDF();
};


void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
	printf("ERROR: error_no=%04X, detail_no=%u\n", (unsigned int)error_no,
		(int)detail_no);
	throw std::exception();
}

void encodeblock(unsigned char in[3], unsigned char out[4], int len)
{
	out[0] = cb64[in[0] >> 2];
	out[1] = cb64[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
	out[2] = (unsigned char)(len > 1 ? cb64[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : '=');
	out[3] = (unsigned char)(len > 2 ? cb64[in[2] & 0x3f] : '=');
}

void encode(FILE *infile, unsigned char *output_buf, int rowcount/*For Percent*/)
{
	unsigned char in[3], out[4];
	int i, len;
	*output_buf = 0;

	while (!feof(infile)) {
		len = 0;
		for (i = 0; i < 3; i++) {
			in[i] = (unsigned char)getc(infile);
			if (!feof(infile))
				len++;
			else
				in[i] = 0;
		}
		if (len) {
			encodeblock(in, out, len);
			strncat((char*)output_buf, (char*)out, 4);
		}
	}
}

size_t read_file()
{
	FILE* hFile = NULL;
	size_t fileSize(0), len(0), buffer_size(0);
	char key = ' ';

	//Open the file and make sure it exists
	hFile = fopen("./email_manager/" FILENAME, "rb");
	if (!hFile) {
		cout << "File not found!!!" << endl;
		_getch();
		exit(EXIT_FAILURE);
	}

	//Get filesize
	fseek(hFile, 0, SEEK_END);
	fileSize = ftell(hFile);
	fseek(hFile, 0, SEEK_SET);

	cout << endl << "Encoding " FILENAME " please be patient..." << endl;

	//Calculate the number of rows in Base64 encoded string.
	//Also calculate the size of the new char to be created for the base64 encode string
	int num_rows = fileSize / SEND_BUF_SIZE + 1;
	int charsize = (num_rows * 72) + (num_rows * 2);

	//Base64 encode image and create encoded_buf string
	unsigned char* b64encode = new unsigned char[charsize];
	*b64encode = 0;
	encode(hFile, b64encode, num_rows /*For Percent*/);
	string encoded_buf = (char*)b64encode;

	//Create structure of email to be sent.
	//ADD_SIZE is number of "things" in the email. 
	fileBuf = new char[ADD_SIZE + num_rows][CHARS];

	//find the date
	time_t now = time(0);
	tm *ltm = localtime(&now);
	theDate = to_string(ltm->tm_mon + 1) + "/" +
		to_string(ltm->tm_mday) + "/" +
		to_string(ltm->tm_year + 1900);

	//string to char
	string temp = "Subject: Danklogger - " + theDate + " report for " + employee + "\r\n";
	char *subject = new char[temp.length() + 1];
	strcpy(subject, temp.c_str());


	//using MIME email format
	//strcpy(fileBuf[len++], "To: " TO "\r\n");
	strcpy(fileBuf[len++], TO);
	buffer_size += strlen(fileBuf[len - 1]);
	strcpy(fileBuf[len++], "From: " FROM "\r\n");
	buffer_size += strlen(fileBuf[len - 1]);
	strcpy(fileBuf[len++], subject);
	buffer_size += strlen(fileBuf[len - 1]);
	strcpy(fileBuf[len++], "Content-Type: application/x-msdownload; name=\"" FILENAME "\"\r\n");
	buffer_size += strlen(fileBuf[len - 1]);
	strcpy(fileBuf[len++], "Content-Transfer-Encoding: base64\r\n");
	buffer_size += strlen(fileBuf[len - 1]);
	strcpy(fileBuf[len++], "Content-Disposition: attachment; filename=\"" FILENAME "\"\r\n");
	buffer_size += strlen(fileBuf[len - 1]);
	strcpy(fileBuf[len++], "\r\n");
	buffer_size += strlen(fileBuf[len - 1]);

	//Attach the Base64 encoded string and set Base64 linesize to 72 characters + \r\n
	int pos = 0;
	string sub_encoded_buf;
	for (int i = 0; i <= num_rows - 1; i++) {
		//Read 72 characters at a time and append \r\n at the end
		sub_encoded_buf = encoded_buf.substr(pos * 72, 72);
		sub_encoded_buf += "\r\n";

		//copy the 72 characters & \r\n to email
		strcpy(fileBuf[len++], sub_encoded_buf.c_str());

		//now increase the buffer_size and increase pos by 1
		buffer_size += sub_encoded_buf.size();
		pos++;
	}

	delete[] b64encode;
	return buffer_size;
}

static size_t fileBuf_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
	struct fileBuf_upload_status *upload_ctx = (struct fileBuf_upload_status *)userp;
	const char *fdata;

	if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
		return 0;
	}

	fdata = fileBuf[upload_ctx->lines_read];

	if (strcmp(fdata, "")) {
		size_t len = strlen(fdata);
		memcpy(ptr, fdata, len);
		upload_ctx->lines_read++;
		return len;
	}

	return 0;
}

void EmailManager::readAnalysisResults()
{
	string temp;
	float percent;

	ifstream config;
	config.open("./email_manager/config.txt");
	if (!config.is_open())
		cout << "can't open config file" << endl;

	config >> temp;
	employee = employee + temp + " ";
	config >> temp;
	employee = employee + temp;
	
	temp.clear();
	config >> email;
	email = "<" + email + ">";
	TO = new char[email.length() + 1];
	strcpy(TO, email.c_str());
	config.close();

	ifstream results;
	results.open("./email_manager/out.txt");
	if (!results.is_open())
		cout << "can't open out file" << endl;

	temp.clear();
	results >> temp;
	totalLT = totalLT + temp + "h ";
	results >> temp;
	totalLT = totalLT + temp + "m ";
	results >> temp;
	totalLT = totalLT + temp + "s";

	results >> temp;
	prodLT = prodLT + temp + "h ";
	results >> temp;
	prodLT = prodLT + temp + "m ";
	results >> temp;
	prodLT = prodLT + temp + "s";

	results >> temp;
	unprodLT = unprodLT + temp + "h ";
	results >> temp;
	unprodLT = unprodLT + temp + "m ";
	results >> temp;
	unprodLT = unprodLT + temp + "s";

	results >> percent;
	percent = percent * 100;
	stringstream ss;
	ss << percent;
	prodScore = ss.str();

	temp.clear();
	results >> temp;
	prog1 = temp;
	results >> percent;
	p1 = percent;

	temp.clear();
	results >> temp;
	prog2 = temp;
	results >> percent;
	p2 = percent;

	temp.clear();
	results >> temp;
	prog3 = temp;
	results >> percent;
	p3 = percent;

	temp.clear();
	results >> temp;
	prog4 = temp;
	results >> percent;
	p4 = percent;

	temp.clear();
	results >> temp;
	prog5 = temp;
	results >> percent;
	p5 = percent;

	//gather swear words
	int i = 0;
	while (true)
	{
		results >> swear[i];
		results >> swearFreq[i];

		numSwears++;
		i++;
		
		if (results.eof())
			break;
	}

	/*for (i = 0; i < 2; i++)
	{
	cout << swear[i] << endl;
	cout << swearFreq[i] << endl;
	}*/
	//cout << numSwears << endl; 

	results.close();

	createPDF();
}


void EmailManager::createPDF()
{
	HPDF_Doc pdf;
	HPDF_Font font;
	HPDF_Page page1;
	HPDF_REAL tw;
	HPDF_REAL height; //841
	HPDF_REAL width;  //595
	HPDF_Point pos;

	pdf = HPDF_New(error_handler, NULL);
	if (!pdf)
		printf("error: cannot create pdf. \n");

	page1 = HPDF_AddPage(pdf);
	height = HPDF_Page_GetHeight(page1);
	width = HPDF_Page_GetWidth(page1);
	font = HPDF_GetFont(pdf, "Times-Roman", NULL);
	HPDF_Page_SetFontAndSize(page1, font, 24);

	//Title
	char* pageTitle = "Danklogger Productivity Report";
	tw = HPDF_Page_TextWidth(page1, pageTitle);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, (width - tw) / 2, height - 70, pageTitle);
	HPDF_Page_EndText(page1);

	//Underline Title
	HPDF_Page_SetLineWidth(page1, 1);
	//For both MoveTo and LineTo, para3 must be same to make a straight line
	HPDF_Page_MoveTo(page1, 142, 765);  //para2 is how far from left margin (x). para3 is (y)
	HPDF_Page_LineTo(page1, 453, 765);  //para2 is how long line is
	HPDF_Page_Stroke(page1);

	//Employee tab
	string tempEmp = "Employee: " + employee;
	char* emp = new char[tempEmp.length() + 1];
	strcpy(emp, tempEmp.c_str());
	HPDF_Page_SetFontAndSize(page1, font, 14);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 50, 715, emp);
	HPDF_Page_EndText(page1);

	//Date tab
	time_t now = time(0);
	tm *ltm = localtime(&now);
	theDate = to_string(ltm->tm_mon + 1) + "/" +
		to_string(ltm->tm_mday) + "/" +
		to_string(ltm->tm_year + 1900);

	string temp = "Date: " + theDate;
	char* date = new char[temp.length() + 1];
	strcpy(date, temp.c_str());

	HPDF_Page_SetFontAndSize(page1, font, 14);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 50, 695, date);
	HPDF_Page_EndText(page1);

	//Total Log Time tab
	temp = "Total Log Time:         " + totalLT;
	char* tlt = new char[temp.length() + 1];
	strcpy(tlt, temp.c_str());
	HPDF_Page_SetFontAndSize(page1, font, 14);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 50, 655, tlt);
	HPDF_Page_EndText(page1);

	//Productive Time tab
	temp = "Productive Time:        " + prodLT;
	char* plt = new char[temp.length() + 1];
	strcpy(plt, temp.c_str());
	HPDF_Page_SetFontAndSize(page1, font, 14);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 50, 635, plt);
	HPDF_Page_EndText(page1);

	//Unproductive Time tab
	temp = "Unproductive Time:    " + unprodLT;
	char* ult = new char[temp.length() + 1];
	strcpy(ult, temp.c_str());
	HPDF_Page_SetFontAndSize(page1, font, 14);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 50, 615, ult);
	HPDF_Page_EndText(page1);

	//Productivity Score tab
	temp = "Productivity Score: " + prodScore + "%";
	char* ps = new char[temp.length() + 1];
	strcpy(ps, temp.c_str());
	HPDF_Page_SetFontAndSize(page1, font, 18);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 50, 575, ps);
	HPDF_Page_EndText(page1);
	//Underline 
	HPDF_Page_MoveTo(page1, 50, 570);  //para2 is how far from left margin (x). para3 is (y)
	HPDF_Page_LineTo(page1, 225, 570);  //para2 is how long line is
	HPDF_Page_Stroke(page1);

	char* top5 = "Top 5 Programs";
	HPDF_Page_SetFontAndSize(page1, font, 14);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 125, 515, top5);
	HPDF_Page_EndText(page1);


	/*
	1 - adjust y coordinate
	2 - adjust x coordinate
	In Arc(page1, x, y, circleSize, angle1, angle2).
	each section will build off each other. start at 0, end at 360
	suppose 1st-45%, 2nd-25%, 3rd-15%, 4th-10%, 5th-5%
	*/

	//Pie chart. r,g,b values must be between 0 and 1.
	//Most used program
	HPDF_Page_SetRGBFill(page1, 0, .322, .816);  //blue
	HPDF_Page_MoveTo(page1, 170, 400);
	HPDF_Page_LineTo(page1, 170, 460);
	//HPDF_Page_Arc(page1, 170, 400, 100, 0, 360*.45);
	HPDF_Page_Arc(page1, 170, 400, 100, 0, 360 * p1);
	pos = HPDF_Page_GetCurrentPos(page1);
	HPDF_Page_LineTo(page1, 170, 400);
	HPDF_Page_Fill(page1);

	//2nd most used
	HPDF_Page_SetRGBFill(page1, .329, .271, .086);  //brown
	HPDF_Page_MoveTo(page1, 170, 400);
	HPDF_Page_LineTo(page1, pos.x, pos.y);
	//HPDF_Page_Arc(page1, 170, 400, 100, 360*.45, 360*.70);
	HPDF_Page_Arc(page1, 170, 400, 100, 360 * p1, 360 * (p1 + p2));
	pos = HPDF_Page_GetCurrentPos(page1);
	HPDF_Page_LineTo(page1, 170, 400);
	HPDF_Page_Fill(page1);

	//3rd most used
	HPDF_Page_SetRGBFill(page1, 1.0, .875, .424);  //tan
	HPDF_Page_MoveTo(page1, 170, 400);
	HPDF_Page_LineTo(page1, pos.x, pos.y);
	//HPDF_Page_Arc(page1, 170, 400, 100, 360*.70, 360*.85);
	HPDF_Page_Arc(page1, 170, 400, 100, 360 * (p1 + p2), 360 * (p1 + p2 + p3));
	pos = HPDF_Page_GetCurrentPos(page1);
	HPDF_Page_LineTo(page1, 170, 400);
	HPDF_Page_Fill(page1);

	//4th most used
	HPDF_Page_SetRGBFill(page1, 0, .875, 1.0);  //teal
	HPDF_Page_MoveTo(page1, 170, 400);
	HPDF_Page_LineTo(page1, pos.x, pos.y);
	//HPDF_Page_Arc(page1, 170, 400, 100, 360*.85, 360*.95);
	HPDF_Page_Arc(page1, 170, 400, 100, 360 * (p1 + p2 + p3), 360 * (p1 + p2 + p3 + p4));
	pos = HPDF_Page_GetCurrentPos(page1);
	HPDF_Page_LineTo(page1, 170, 400);
	HPDF_Page_Fill(page1);

	//5th most used  
	HPDF_Page_SetRGBFill(page1, .290, .275, .247);  //grey
	HPDF_Page_MoveTo(page1, 170, 400);
	HPDF_Page_LineTo(page1, pos.x, pos.y);
	//HPDF_Page_Arc(page1, 170, 400, 100, 360*.95, 360);
	HPDF_Page_Arc(page1, 170, 400, 100, 360 * (p1 + p2 + p3 + p4), 360);
	pos = HPDF_Page_GetCurrentPos(page1);
	HPDF_Page_LineTo(page1, 170, 400);
	HPDF_Page_Fill(page1);


	stringstream stream1;
	stringstream stream2;
	stringstream stream3;
	stringstream stream4;
	stringstream stream5;

	//draw squares. match program name with percents
	HPDF_Page_SetLineWidth(page1, 0);

	HPDF_Page_SetRGBFill(page1, 0, .322, .816);
	HPDF_Page_Rectangle(page1, 325, 465, 25, 25);
	HPDF_Page_FillStroke(page1);
	stream1 << setprecision(0) << p1 * 100;
	string temp1 = prog1 + " - " + stream1.str() + "%";
	char* prog1 = new char[temp1.length() + 1];
	strcpy(prog1, temp1.c_str());
	HPDF_Page_SetRGBFill(page1, 0, 0, 0);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 360, 470, prog1);
	HPDF_Page_EndText(page1);

	HPDF_Page_SetRGBFill(page1, .329, .271, .086);
	HPDF_Page_Rectangle(page1, 325, 425, 25, 25);
	HPDF_Page_FillStroke(page1);
	stream2 << setprecision(0) << p2 * 100;
	string temp2 = prog2 + " - " + stream2.str() + "%";
	char* prog2 = new char[temp2.length() + 1];
	strcpy(prog2, temp2.c_str());
	HPDF_Page_SetRGBFill(page1, 0, 0, 0);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 360, 430, prog2);
	HPDF_Page_EndText(page1);

	HPDF_Page_SetRGBFill(page1, 1.0, .875, .424);
	HPDF_Page_Rectangle(page1, 325, 385, 25, 25);
	HPDF_Page_FillStroke(page1);
	stream3 << fixed << setprecision(0) << p3 * 100;
	string temp3 = prog3 + " - " + stream3.str() + "%";
	char* prog3 = new char[temp3.length() + 1];
	strcpy(prog3, temp3.c_str());
	HPDF_Page_SetRGBFill(page1, 0, 0, 0);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 360, 390, prog3);
	HPDF_Page_EndText(page1);

	HPDF_Page_SetRGBFill(page1, 0, .875, 1.0);
	HPDF_Page_Rectangle(page1, 325, 345, 25, 25);
	HPDF_Page_FillStroke(page1);
	stream4 << fixed << setprecision(0) << p4 * 100;
	string temp4 = prog4 + " - " + stream4.str() + "%";
	char* prog4 = new char[temp4.length() + 1];
	strcpy(prog4, temp4.c_str());
	HPDF_Page_SetRGBFill(page1, 0, 0, 0);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 360, 350, prog4);
	HPDF_Page_EndText(page1);

	HPDF_Page_SetRGBFill(page1, .290, .275, .247);
	HPDF_Page_Rectangle(page1, 325, 305, 25, 25);
	HPDF_Page_FillStroke(page1);
	stream5 << fixed << setprecision(0) << p5 * 100;
	string temp5 = prog5 + " - " + stream5.str() + "%";
	char* prog5 = new char[temp5.length() + 1];
	strcpy(prog5, temp5.c_str());
	HPDF_Page_SetRGBFill(page1, 0, 0, 0);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 360, 310, prog5);
	HPDF_Page_EndText(page1);


	//go back to black font and 1px line
	HPDF_Page_SetRGBFill(page1, 0, 0, 0);
	HPDF_Page_SetLineWidth(page1, 1);

	//Unprofessional Words tab
	char* words = "Unprofessional Words Detected";
	HPDF_Page_SetFontAndSize(page1, font, 18);
	HPDF_Page_BeginText(page1);
	HPDF_Page_TextOut(page1, 50, 200, words);
	HPDF_Page_EndText(page1);
	HPDF_Page_MoveTo(page1, 50, 195);
	HPDF_Page_LineTo(page1, 282, 195);
	HPDF_Page_Stroke(page1);

	HPDF_Page_SetFontAndSize(page1, font, 14);
	HPDF_REAL xCoord = 50;
	HPDF_REAL yCoord = 160;
	for (int i = 0; i < numSwears; i++)
	{
		string tempy = swear[i] + "(" + swearFreq[i] + "x),";
		char* finalSwear = new char[tempy.length() + 1];
		strcpy(finalSwear, tempy.c_str());
		//cout << tempy << endl;
		//cout << tempy.length() + 1 << endl;

		HPDF_Page_BeginText(page1);
		if (xCoord + tempy.length()*6.5 > 550)
		{
			//start a newline, then print
			xCoord = 50;
			yCoord -= 25;
			HPDF_Page_TextOut(page1, xCoord, yCoord, finalSwear);
			xCoord = xCoord + tempy.length()*6.5;
		}
		else
		{
			HPDF_Page_TextOut(page1, xCoord, yCoord, finalSwear);
			xCoord = xCoord + tempy.length()*6.5;
		}
		HPDF_Page_EndText(page1);
	}

	HPDF_SaveToFile(pdf, "./email_manager/" FILENAME);
	HPDF_Free(pdf);

	//emailPDF();
}

void EmailManager::emailPDF()
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	struct curl_slist *recipients = NULL;
	struct fileBuf_upload_status file_upload_ctx;
	size_t file_size(0);

	file_upload_ctx.lines_read = 0;
	curl = curl_easy_init();
	file_size = read_file();

	//send the email
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_USERNAME, "dankestlogger");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "redbullcheezit");
		curl_easy_setopt(curl, CURLOPT_URL, "smtp.gmail.com:587");
		curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		//curl_easy_setopt(curl, CURLOPT_CAINFO, "google.pem");
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);
		recipients = curl_slist_append(recipients, TO);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, file_size);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, fileBuf_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &file_upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);  //prints what's going on

		res = curl_easy_perform(curl);

		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		curl_slist_free_all(recipients);
		curl_easy_cleanup(curl);
	}
	delete[] fileBuf;
}
