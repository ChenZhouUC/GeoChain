#ifndef _TIMER_
#define _TIMER_

#include <time.h>
#include <iomanip>
#include "LogGuardian.h"
#include "Global.h"

namespace GeoChain {
namespace Utils {
class Timer {
 private:
	clock_t begin_;
	clock_t end_;

 public:
	Timer() { this->begin_ = clock(); };
	~Timer() {
		this->end_ = clock();
		LOG(INFO) << std::setprecision(g_GlobalVars.visualize_precision)
							<< "time cost: " << (double)(this->end_ - this->begin_) / CLOCKS_PER_SEC << " s";
	};
};

class ExperimentalTimer {
 private:
	clock_t begin_;
	clock_t end_;
	int experiments_;

 public:
	ExperimentalTimer(int experiment_num) : experiments_(experiment_num) { this->begin_ = clock(); };
	~ExperimentalTimer() {
		this->end_ = clock();
		LOG(INFO) << std::setprecision(g_GlobalVars.visualize_precision) << "time cost per experiment: "
							<< (double)(this->end_ - this->begin_) / (this->experiments_ * CLOCKS_PER_SEC) << " s";
	};
};

}	// namespace Utils
}	// namespace GeoChain

#endif