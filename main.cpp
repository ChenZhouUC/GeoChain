#include "Utils/ConfigLoader.h"

GeoChain::Utils::GlobalVar globalVars;
GeoChain::Utils::GlobalKey globalKeys;

int main(int argc, char **argv) {
	GeoChain::Utils::DefineGlobalKeys();
	GeoChain::Utils::LogGuardian logger =
			GeoChain::Utils::LogGuardian(argc, argv);

	Json::Value root_config;
	std::string json_config = "./config.json";
	GeoChain::Utils::ReadJsonFile(json_config, root_config);

	GeoChain::Utils::LoadingConfig(root_config);
	LOG(INFO) << root_config;
}