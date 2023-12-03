#pragma once

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Window.h"

namespace Luha {

	struct ApplicationSpecification
	{
		char*    Name              = "Luha App";
		uint32_t Window_Width      = 1600;
		uint32_t Window_Height     = 900;
		uint32_t Window_Min_Width  = 0;
		uint32_t Window_Min_Height = 0;
		uint32_t Window_Max_Width  = 0;
		uint32_t Window_Max_Height = 0;
		bool     Window_Resizeable = true;
		bool     VSync             = true;

	};
	
	class Application
	{
	public:
		Application(const ApplicationSpecification& spec = ApplicationSpecification());
		virtual ~Application();

		void Run();
		void Close();
		void OnEvent(Event& e);

		inline static Application& Get() { return *s_Instance; }
		float GetTime() const;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		Scope<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}