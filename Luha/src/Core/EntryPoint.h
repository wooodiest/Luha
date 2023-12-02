#pragma once

// Application definition
#ifdef LH_PLATFORM_WINDOWS

	#if defined(LH_DIST) && LH_CONSOLE_IN_DISTRIBUTION_BUILD

	#else
		int main(int argc, char** argv)
		{
			Luha::Log::Init();
			LH_CORE_INFO("Hello World");
			return 0;
		}
	#endif


#endif // LH_PLATFORM_WINDOWS

