#pragma once

/* Client log macros

	LH_TRACE(...)   
	LH_INFO(...)    
	LH_WARN(...)    
	LH_ERROR(...)   
	LH_CRITICAL(...)

*/

#include "Base.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Luha {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#if defined(LH_DIST) && (LH_LOG_IN_DISTRIBUTION_BUILD == 0)
	// Core log macros
	#define LH_CORE_TRACE(...)    
	#define LH_CORE_INFO(...)     
	#define LH_CORE_WARN(...)     
	#define LH_CORE_ERROR(...)    
	#define LH_CORE_CRITICAL(...) 
	
	// Client log macros
	#define LH_TRACE(...)         
	#define LH_INFO(...)          
	#define LH_WARN(...)          
	#define LH_ERROR(...)         
	#define LH_CRITICAL(...)      

#elif (LH_LOG_TO_CONSOLE == 0 && LH_LOG_TO_FILE == 0)
	// Core log macros
	#define LH_CORE_TRACE(...)    
	#define LH_CORE_INFO(...)     
	#define LH_CORE_WARN(...)     
	#define LH_CORE_ERROR(...)    
	#define LH_CORE_CRITICAL(...) 
	
	// Client log macros
	#define LH_TRACE(...)         
	#define LH_INFO(...)          
	#define LH_WARN(...)          
	#define LH_ERROR(...)         
	#define LH_CRITICAL(...) 

#else
	// Core log macros
	#define LH_CORE_TRACE(...)    ::Luha::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define LH_CORE_INFO(...)     ::Luha::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define LH_CORE_WARN(...)     ::Luha::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define LH_CORE_ERROR(...)    ::Luha::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define LH_CORE_CRITICAL(...) ::Luha::Log::GetCoreLogger()->critical(__VA_ARGS__)
	
	// Client log macros
	#define LH_TRACE(...)         ::Luha::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define LH_INFO(...)          ::Luha::Log::GetClientLogger()->info(__VA_ARGS__)
	#define LH_WARN(...)          ::Luha::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define LH_ERROR(...)         ::Luha::Log::GetClientLogger()->error(__VA_ARGS__)
	#define LH_CRITICAL(...)      ::Luha::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif
