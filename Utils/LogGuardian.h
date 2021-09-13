#ifndef _LOGGUARDIAN_
#define _LOGGUARDIAN_

#include <glog/logging.h>

// #define GOOGLE_GLOG_DLL_DECL
#define GLOG_NO_ABBREVIATED_SEVERITIES

// #ifdef DEBUG
// #pragma comment(lib, "glog/libglog_static_d.lib")
// #else
#pragma comment(lib, "glog/libglog_static.lib")
// #endif

namespace GeoChain {
namespace Utils {

class LogGuardian {
 public:
	LogGuardian(int argc, char** argv) {
		// programme name
		google::InitGoogleLogging(argv[0]);

		// FLAGS_logtostderr = false;
		FLAGS_alsologtostderr = true;
		FLAGS_colorlogtostderr = true;
		FLAGS_log_prefix = true;
		FLAGS_logbufsecs = 0;			 // s
		FLAGS_max_log_size = 100;	// MB
		FLAGS_stop_logging_if_full_disk = true;

		FLAGS_log_dir = "./logs/";
		std::string mkdir_command = "mkdir -p " + FLAGS_log_dir;
		system(mkdir_command.c_str());

		// // logging output level & storage prefix definition
		// std::stringstream prefix;
		// prefix << FLAGS_log_dir << argv[0];

		// google::SetLogDestination(google::GLOG_INFO, (prefix.str() +
		// "_INFO").c_str()); google::SetLogDestination(google::GLOG_WARNING,
		// (prefix.str() + "_WARNING").c_str());
		// google::SetLogDestination(google::GLOG_ERROR, (prefix.str() +
		// "_ERROR").c_str()); google::SetLogDestination(google::GLOG_FATAL,
		// (prefix.str() + "_FATAL").c_str());

		// extension between prefix and timestamp
		// google::SetLogFilenameExtension("@");
		// stdoutput level
		google::SetStderrLogging(google::GLOG_INFO);
	}
	~LogGuardian() { google::ShutdownGoogleLogging(); }
};

}	// namespace Utils
}	// namespace GeoChain

#endif