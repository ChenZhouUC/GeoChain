#include "Utils/ConfigLoader.h"

GeoChain::Utils::GlobalVar g_GlobalVars;
GeoChain::Utils::GlobalKey g_GlobalKeys;

const std::string g_JsonConfigPath = "./config.json";
Json::Value g_ConfigRoot;

int main(int argc, char **argv) {
	// initiate logger
	GeoChain::Utils::LogGuardian g_Logger = GeoChain::Utils::LogGuardian(argc, argv);
	// define global config keys
	GeoChain::Utils::DefineGlobalKeys();
	// load config file into global variables
	GeoChain::Utils::ReadJsonFile(g_JsonConfigPath, g_ConfigRoot);
	GeoChain::Utils::LoadingConfig(g_ConfigRoot);
	LOG(INFO) << g_ConfigRoot;
}