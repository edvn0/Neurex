#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Neurex {

	class NX_API Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger> get_core_logger();
		inline static std::shared_ptr<spdlog::logger> get_client_logger();
	private:
		static std::shared_ptr<spdlog::logger> core_logger;
		static std::shared_ptr<spdlog::logger> client_logger;
	};

}

#define NX_CORE_ERROR(...) ::Neurex::Logger::get_core_logger()->error(__VA_ARGS__)
#define NX_CORE_WARN(...) ::Neurex::Logger::get_core_logger()->warn(__VA_ARGS__)
#define NX_CORE_INFO(...) ::Neurex::Logger::get_core_logger()->info(__VA_ARGS__)
#define NX_CORE_TRACE(...) ::Neurex::Logger::get_core_logger()->trace(__VA_ARGS__)


#define NX_ERROR(...) ::Neurex::Logger::get_client_logger()->error(__VA_ARGS__)
#define NX_WARN(...) ::Neurex::Logger::get_client_logger()->warn(__VA_ARGS__)
#define NX_INFO(...) ::Neurex::Logger::get_client_logger()->info(__VA_ARGS__)
#define NX_TRACE(...) ::Neurex::Logger::get_client_logger()->trace(__VA_ARGS__)
