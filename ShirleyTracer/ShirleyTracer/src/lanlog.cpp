#include "lanlog.h"

namespace lanlog {


	void initLogging() {
		f.open("D:\\sample\\ShirleyTracer\\ShirleyTracer\\ShirleyTracer\\LOG\\info.txt");
		//		error_stream.open("D:\\sample\\ShirleyTracer\\ShirleyTracer\\ShirleyTracer\\warning.txt");
		warning_stream.open("D:\\sample\\ShirleyTracer\\ShirleyTracer\\ShirleyTracer\\error.txt");
		init_already = 1;
	}

	void log_info(string s) {
		assert("log system uninitialized", init_already == 1);

		f << s;
		f << endl;
	}

	void log_vec3(vec3 ra) {
//		void log_vec3(vec3 ra);
		lanlog::log_info(to_string(ra.x()) + " " + to_string(ra.y()) + " " + to_string(ra.z()));
	}

	void log_error(string s) {
		warning_stream << s;
		warning_stream << endl;
	}

	void endLogging() {
		f.close();
		//		error_stream.close();
		warning_stream.close();
	}
}