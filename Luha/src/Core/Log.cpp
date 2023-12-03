#include "lhpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

#include "Core/ApplicationSettings.h"

namespace Luha {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
#if defined(LH_DIST)
	// Distribiution version
	#if LH_LOG_IN_DISTRIBUTION_BUILD == 1
		// Logs are enabled
		#if LH_CONSOLE_IN_DISTRIBUTION_BUILD == 1
			// console is visible
			#if LH_LOG_TO_CONSOLE == 1 && LH_LOG_TO_FILE == 1
				// Logs to console and file	
				std::vector<spdlog::sink_ptr> logSinks;
				logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
				logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Luha.log", true));

				logSinks[0]->set_pattern("%^[%T] %n: %v%$");
				logSinks[1]->set_pattern("[%T] [%l] %n: %v");

				s_CoreLogger = std::make_shared<spdlog::logger>("Luha", begin(logSinks), end(logSinks));
				spdlog::register_logger(s_CoreLogger);
				s_CoreLogger->set_level(spdlog::level::trace);
				s_CoreLogger->flush_on(spdlog::level::trace);

				s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
				spdlog::register_logger(s_ClientLogger);
				s_ClientLogger->set_level(spdlog::level::trace);
				s_ClientLogger->flush_on(spdlog::level::trace);

			#elif LH_LOG_TO_CONSOLE == 1 && LH_LOG_TO_FILE == 0
				// Logs to console
				spdlog::set_pattern("%^[%T] %n: %v%$");

				s_CoreLogger = spdlog::stdout_color_mt("Luha");
				s_CoreLogger->set_level(spdlog::level::trace);

				s_ClientLogger = spdlog::stdout_color_mt("APP");
				s_ClientLogger->set_level(spdlog::level::trace);

			#elif LH_LOG_TO_CONSOLE == 0 && LH_LOG_TO_FILE == 1
				// Logs to file
				std::vector<spdlog::sink_ptr> logSinks;
				logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Luha.log", true));
				logSinks[0]->set_pattern("[%T] [%l] %n: %v");

				s_CoreLogger = std::make_shared<spdlog::logger>("Luha", begin(logSinks), end(logSinks));
				spdlog::register_logger(s_CoreLogger);
				s_CoreLogger->set_level(spdlog::level::trace);
				s_CoreLogger->flush_on(spdlog::level::trace);

				s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
				spdlog::register_logger(s_ClientLogger);
				s_ClientLogger->set_level(spdlog::level::trace);
				s_ClientLogger->flush_on(spdlog::level::trace);

			#else
				// No logs
			#endif
		#elif LH_LOG_TO_FILE == 1
			// Logs to file
			std::vector<spdlog::sink_ptr> logSinks;
			logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Luha.log", true));
			logSinks[0]->set_pattern("[%T] [%l] %n: %v");

			s_CoreLogger = std::make_shared<spdlog::logger>("Luha", begin(logSinks), end(logSinks));
			spdlog::register_logger(s_CoreLogger);
			s_CoreLogger->set_level(spdlog::level::trace);
			s_CoreLogger->flush_on(spdlog::level::trace);

			s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
			spdlog::register_logger(s_ClientLogger);
			s_ClientLogger->set_level(spdlog::level::trace);
			s_ClientLogger->flush_on(spdlog::level::trace);

		#endif
	#else
		// No logs
	#endif
#else
	// Normal logs settings
	#if LH_LOG_TO_CONSOLE == 1 && LH_LOG_TO_FILE == 1
		// Logs to console and file	
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Luha.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("Luha", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);

	#elif LH_LOG_TO_CONSOLE == 1 && LH_LOG_TO_FILE == 0
		// Logs to console
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("Luha");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

	#elif LH_LOG_TO_CONSOLE == 0 && LH_LOG_TO_FILE == 1
		// Logs to file
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Luha.log", true));
		logSinks[0]->set_pattern("[%T] [%l] %n: %v");

		s_CoreLogger = std::make_shared<spdlog::logger>("Luha", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);

	#else
		// No logs
	#endif
#endif
	}

}