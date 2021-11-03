#pragma once

// clang-format off
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
//clang-format on


namespace Engine {

class Logger {
public:
	static void init();

	static std::shared_ptr<spdlog::logger> get_core_logger();
	static std::shared_ptr<spdlog::logger> get_client_logger();

private:
	static std::shared_ptr<spdlog::logger> core_logger;
	static std::shared_ptr<spdlog::logger> client_logger;
};

}

#define NX_CORE_ERROR(...) ::Engine::Logger::get_core_logger()->error(__VA_ARGS__)
#define NX_CORE_WARN(...) ::Engine::Logger::get_core_logger()->warn(__VA_ARGS__)
#define NX_CORE_INFO(...) ::Engine::Logger::get_core_logger()->info(__VA_ARGS__)
#define NX_CORE_TRACE(...) ::Engine::Logger::get_core_logger()->trace(__VA_ARGS__)

#define NX_ERROR(...) ::Engine::Logger::get_client_logger()->error(__VA_ARGS__)
#define NX_WARN(...) ::Engine::Logger::get_client_logger()->warn(__VA_ARGS__)
#define NX_INFO(...) ::Engine::Logger::get_client_logger()->info(__VA_ARGS__)
#define NX_TRACE(...) ::Engine::Logger::get_client_logger()->trace(__VA_ARGS__)
