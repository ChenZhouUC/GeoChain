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
}	// namespace Utils
}	// namespace GeoChain