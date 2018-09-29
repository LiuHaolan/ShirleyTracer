#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "stdio.h"
#include "assert.h"
using namespace std;

#define LOG(format) LOG_##format
#define LOG_INFO lanlog::f;
#define LOG_ERROR lanlog::error_stream;
#define LOG_WARNING lanlog:warning_stream;

namespace lanlog {

	static int init_already = 0;

	static ofstream f;
	static ofstream error_stream;
	static ofstream warning_stream;
	
	void initLogging();
	
	void log_info(string s);

	void log_error(string s);

	void endLogging();
}



