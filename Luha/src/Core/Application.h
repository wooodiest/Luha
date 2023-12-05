#pragma once

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "glm/glm.hpp"

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
		bool     MenuBar           = true;

	};
	
	class Application
	{
	public:
		Application(const ApplicationSpecification& spec = ApplicationSpecification());
		virtual ~Application();

		void Run();
		void Close();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
		float GetTime() const;
		Timestep GetDeltaTime() const;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		ApplicationSpecification m_ApplicationSpecification;
		Scope<Window> m_Window;
		LayerStack m_LayerStack;

		glm::vec4 m_ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
		bool m_Running = true;
		bool m_Minimized = false;
		Timestep m_LastFrameTime = 0.0f;
		Timestep m_DeltaTime = 0.0f;

	private:
		void InitImGui();
		void ShutdownImGui();
		void BeginImGuiFrame();
		void EndImGuiFrame();
		void BeginImGuiMainDockingWindow();
		void EndImGuiMainDockingWindow();

		void OnApplicationMainMenuRender();

	private:
		static Application* s_Instance;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}