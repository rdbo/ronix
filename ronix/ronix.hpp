#ifndef RONIX_HPP
#define RONIX_HPP

#include "pch.hpp"
#include "utils/logger.hpp"
#include "utils/vmtmgr.hpp"
#include "config.hpp"

namespace Ronix {
	void Init();
	void Shutdown();

	namespace Data {
		extern bool hasShutdown;
		extern std::unique_ptr<Logger> logger;
		extern std::unique_ptr<Config> config;
	}

	namespace Hacks {

	}

	namespace Hooks {

	}
}

#define RONIX_LOG Ronix::Data::logger->Log
#define RONIX_CFG Ronix::Data::config->GetData()

#endif
