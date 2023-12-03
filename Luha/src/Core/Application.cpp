#include "lhpch.h"

#include "Core/Application.h"
#include "Core/Timestep.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Luha {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
		: m_ApplicationSpecification(spec)
	{
		LH_PROFILE_FUNCTION();

		Log::Init();

		s_Instance = this;
		m_Window = CreateScope<Window>(spec);
		m_Window->SetEventCallback(LH_BIND_EVENT_FN(Application::OnEvent));

		InitImGui();
	}

	Application::~Application()
	{
		LH_PROFILE_FUNCTION();

		Close();
	}

	void Application::Run()
	{
		LH_PROFILE_FUNCTION();

		while (m_Running)
		{
			LH_PROFILE_SCOPE("RunLoop");

			// Renderer commands
			glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
			glClear(GL_COLOR_BUFFER_BIT);

			// Calculate delta time
			float time = GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Application
			BeginImGuiFrame();
			if (!m_Minimized)
			{
				// Layer stack OnUpdate
				{
					LH_PROFILE_SCOPE("Layer stack OnUpdate()");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
				
				// Main menu
				{
					LH_PROFILE_SCOPE("Main menu render");

					if (m_ApplicationSpecification.MenuBar && ImGui::BeginMainMenuBar())
					{
						OnApplicationMainMenuRender();

						for (Layer* layer : m_LayerStack)
							layer->OnMainMenuRender();

						ImGui::EndMainMenuBar();
					}
				}
				// Layer stack OnImGuiRender
				{
					LH_PROFILE_SCOPE("Layer stack OnImGuiRender");

					BeginImGuiMainDockingWindow();

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();

					EndImGuiMainDockingWindow();
				}
			}
			EndImGuiFrame();

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		LH_PROFILE_FUNCTION();

		ShutdownImGui();
		s_Instance = nullptr;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(LH_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(LH_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	float Application::GetTime() const
	{
		return (float)glfwGetTime();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		LH_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		return false;
	}

	void Application::InitImGui()
	{
		LH_PROFILE_FUNCTION();
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* window = Application::Get().GetWindow().GetNativeWindow();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void Application::ShutdownImGui()
	{
		LH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::BeginImGuiFrame()
	{
		LH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Application::EndImGuiFrame()
	{
		LH_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Application::BeginImGuiMainDockingWindow()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		if (m_ApplicationSpecification.MenuBar)
			window_flags |= ImGuiWindowFlags_MenuBar;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);

		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Begin("DockSpaceWindow", nullptr, window_flags);
		ImGui::PopStyleColor(); // MenuBarBg
		ImGui::PopStyleVar(2);
		ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		ImGui::DockSpace(ImGui::GetID("MyDockspace"));
		style.WindowMinSize.x = minWinSizeX;
	}

	void Application::EndImGuiMainDockingWindow()
	{
		ImGui::End();
	}

	void Application::OnApplicationMainMenuRender()
	{
		if(ImGui::BeginMenu("Window"))
		{
			// Vsync
			ImGui::SeparatorText("  Window  ");
			bool vsync = GetWindow().IsVSync();
			if(ImGui::Checkbox("Vsync", &vsync))
			{
				GetWindow().SetVSync(vsync);
			}

			ImGui::SeparatorText("  Application  ");
			// Close
			if (ImGui::Button("Close"))
			{
				m_Running = false;
			}
			// ...

			ImGui::EndMenu();
		}
	}

}