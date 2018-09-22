#pragma once

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#define LOG(format) LOG_##format
#define LOG_INFO lanlog::f;
#define LOG_ERROR lanlog::error_stream;
#define LOG_WARNING lanlog:warning_stream;

namespace lanlog {


	static ofstream f;
//	static ofstream error_stream;
//	static ofstream warning_stream;
	
	void initLogging() {
		f.open("D:\\sample\\ShirleyTracer\\ShirleyTracer\\ShirleyTracer\\LOG\\info.txt");
	//	error_stream.open("D:\\sample\\ShirleyTracer\\ShirleyTracer\\ShirleyTracer\\warning.txt");
	//	warning_stream.open("D:\\sample\\ShirleyTracer\\ShirleyTracer\\ShirleyTracer\\error.txt");
		
	}
	
	void log_info(string s) {
		f << s;
		f << endl;
	}

	void endLogging() {
		f.close();
	//	error_stream.close();
	//	warning_stream.close();
	}
}



