#pragma once

extern Luha::Application* Luha::CreateApplication(int argc, char** argv);

namespace Luha {

	int Main(int argc, char** argv)
	{
		Luha::Application* app = Luha::CreateApplication(argc, argv);
		app->Run();
		delete app;
		
		return 0;
	}

}

// Application definition
#ifdef LH_PLATFORM_WINDOWS

	#if defined(LH_DIST) && LH_CONSOLE_IN_DISTRIBUTION_BUILD == 0
		// To open without console:
		// In Visual studio 2022 go to Project:Properties -> Linker -> System -> change SubSystem to Windows (/SUBSYSTEM:WINDOWS)
		// To undo changes change SubSystem to Console (/SUBSYSTEM:CONSOLE)
		
		#include <Windows.h>
		int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
		{
			return Luha::Main(__argc, __argv);
		}
		
	#else
		int main(int argc, char** argv)
		{
			return Luha::Main(argc, argv);
		}
	#endif


#endif // LH_PLATFORM_WINDOWS

