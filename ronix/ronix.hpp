#ifndef RONIX_HPP
#define RONIX_HPP

#include "pch.hpp"
#include "utils/logger.hpp"
#include "utils/vmtmgr.hpp"
#include "config.hpp"
#include "cstrike.hpp"

namespace Ronix {
	void Init();
	void Shutdown();

	namespace Data {
		extern bool hasShutdown;
		extern std::unique_ptr<Logger> logger;
		extern std::unique_ptr<Config> config;
		extern std::unique_ptr<cstrike_t> cstrike;
		extern std::unique_ptr<VmtMgr> BaseClientDllVmt;
		extern std::unique_ptr<VmtMgr> ModelRenderVmt;
	}

	namespace Hacks {

	}

	namespace Hooks {

	}
}

#define RONIX_LOG Ronix::Data::logger->Log

#endif
