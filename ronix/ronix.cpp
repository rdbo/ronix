#include "ronix.hpp"

bool Ronix::Data::hasShutdown = false;
std::unique_ptr<Logger> Ronix::Data::logger;
std::unique_ptr<Config> Ronix::Data::config;

void Ronix::Init()
{
	Ronix::Data::logger = std::unique_ptr<Logger>(new Logger("/tmp/ronix.log"));
	RONIX_LOG("Loaded\n");
	
	Ronix::Data::config = std::unique_ptr<Config>(new Config());
	RONIX_CFG.bhopEnable = true;
	Ronix::Data::config->Save("test");
	RONIX_CFG.bhopEnable = false;
	Ronix::Data::config->Load("test");
	RONIX_LOG("Bhop Enable: %s\n", RONIX_CFG.bhopEnable ? "true" : "false");
}

void Ronix::Shutdown()
{
	if (Ronix::Data::hasShutdown)
		return;
	
	Ronix::Data::hasShutdown = true;
	RONIX_LOG("Unloaded\n");
}

void __attribute__((constructor)) ronix_entry()
{
	Ronix::Init();
}

void __attribute__((destructor)) ronix_exit()
{
	// Ronix::Shutdown();
}
