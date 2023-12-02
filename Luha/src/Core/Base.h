#pragma once

// Aditional application settings
#define LH_CONSOLE_IN_DISTRIBUTION_BUILD 0
#define LH_LOG_IN_DISTRIBUTION_BUILD 1
#define LH_LOG_TO_CONSOLE 1
#define LH_LOG_TO_FILE 1

// Includes 
#include "PlatformDetection.h"
#include "Log.h"
#include "Assert.h"
#include <memory>

// Useful defines
#define BIT(x) (1 << x)
#define LH_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Luha {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
