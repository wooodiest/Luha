#pragma once

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "glm/glm.hpp"
#include "imgui.h"

namespace Luha {

	enum class AppColorTheme
	{
		Dark = 1, Classic, Light
	};

	// TODO load all fonts in font dir instead of hardcoding them
	enum class AppFont
	{
		Roboto = 1, OpenSans, Oswald, Montserrat
	};

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
		bool     VSync             = true; // TODO: Custom frame lock
		bool     MenuBar           = true;
		AppColorTheme ColorThema   = AppColorTheme::Dark;
		AppFont       Font         = AppFont::Roboto;
		float         FontSize     = 18.0f;
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

		ImFont* m_Font = nullptr;
		bool m_FontChanged = false;

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
		void SetImGuiTheme();

		void OnApplicationMainMenuRender();
		void LoadFont();

	private:
		static Application* s_Instance;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}