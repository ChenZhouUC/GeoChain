#include "Utils/ConfigLoader.h"
#include "Elements/ElementaryElements.h"

GeoChain::Utils::GlobalVar g_GlobalVars;
GeoChain::Utils::GlobalKey g_GlobalKeys;

const std::string g_JsonConfigPath = "./config.json";
Json::Value g_ConfigRoot;

int main(int argc, char **argv) {
	// initiate logger
	GeoChain::Utils::LogGuardian g_Logger(argc, argv);
	// define global config keys
	GeoChain::Utils::DefineGlobalKeys();
	// load config file into global variables
	if (!GeoChain::Utils::ReadJsonFile(g_JsonConfigPath, g_ConfigRoot)) {
		LOG(FATAL) << "[FATAL] ===> Reading config file failed! Exit with code 1.";
		return 1;
	} else {
		GeoChain::Utils::LoadingConfig(g_ConfigRoot);
		LOG(INFO) << g_ConfigRoot;
	}

	GeoChain::Euclidean::Point center(GeoChain::Euclidean::EUC2D, 0, 1);
	float theta = M_PI_4;
	GeoChain::Euclidean::HalfLine line(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, center, theta);

	line.Describe();
	line.Maturate();
	line.Describe();

	GeoChain::Euclidean::HalfLine line1(GeoChain::Euclidean::EUC2D, GeoChain::Euclidean::DESC, center, -theta);

	line1.Describe();
	line1.Maturate();
	line1.Describe();

	GeoChain::Euclidean::Line line2(GeoChain::Euclidean::EUC2D, 1, 2, 3);
	line2.Describe();
	line2.Maturate();
	line2.Describe();
}