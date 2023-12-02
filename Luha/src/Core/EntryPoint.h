#pragma once
#include <stdio.h>

// Application definition
#ifdef LH_PLATFORM_WINDOWS

	#if defined(LH_DIST) && LH_CONSOLE_IN_DISTRIBUTION_BUILD

	#else
		int main(int argc, char** argv)
		{
			printf("Hello World!");

			return 0;
		}
	#endif


#endif // LH_PLATFORM_WINDOWS

