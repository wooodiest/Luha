#include "lhpch.h"

#include "Core/Application.h"
#include "Core/Timestep.h"
#include "Utils/Time.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "implot.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Utils/Random.h"

namespace Luha {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
	{
		LH_PROFILE_FUNCTION();

		Log::Init();

		s_Instance = this;

		// try load spec from file
		// else 
		m_ApplicationSpecification = spec;

		m_Window = CreateScope<Window>(spec);
		m_Window->SetEventCallback(LH_BIND_EVENT_FN(Application::OnEvent));

		InitImGui();
		Random::Init();;
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
			Timestep time = GetTime();
			m_DeltaTime = time - m_LastFrameTime;
			m_DeltaTime = m_DeltaTime > 0.016f ? 0.016f : m_DeltaTime;
			m_LastFrameTime = time;

			// -> Application
			// Layer stack OnUpdate
			{
				LH_PROFILE_SCOPE("Layer stack OnUpdate()");
				#ifdef LH_UPDATE_ON_MINIMIZED
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(m_DeltaTime);
				#else
				if (!m_Minimized)
				{
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(m_DeltaTime);
				}
				#endif					
			}

			if (m_FontChanged)
				LoadFont();

			BeginImGuiFrame();
			ImGui::PushFont(m_Font);
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
			ImGui::PopFont();
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

	Timestep Application::GetDeltaTime() const
	{
		return m_DeltaTime;
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
		m_ApplicationSpecification.Window_Height = e.GetHeight();
		m_ApplicationSpecification.Window_Width = e.GetWidth();
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
		SetImGuiTheme();

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

		io.Fonts->AddFontDefault();
		LoadFont();
		
		ImPlot::CreateContext();
	}

	void Application::ShutdownImGui()
	{
		LH_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
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

	void Application::SetImGuiTheme()
	{
		switch (m_ApplicationSpecification.ColorThema)
		{
			case AppColorTheme::Dark: ImGui::StyleColorsDark(); break;
			case AppColorTheme::Classic: ImGui::StyleColorsClassic(); break;
			case AppColorTheme::Light: ImGui::StyleColorsLight(); break;
			default: ImGui::StyleColorsDark(); break;
		}
	}

	void Application::OnApplicationMainMenuRender()
	{
		if(ImGui::BeginMenu("Application"))
		{
			// Save
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				//TODO
			}

			ImGui::Separator();

			// Vsync
			bool vsync = GetWindow().IsVSync();
			if (ImGui::MenuItem("Vsync", "", &vsync))
			{
				GetWindow().SetVSync(vsync);
			}

			ImGui::Separator();

			// Color theme
			if (ImGui::BeginMenu("Color theme"))
			{
				int selected = static_cast<int>(m_ApplicationSpecification.ColorThema);

				if (ImGui::Selectable("Dark", selected == 1))
				{
					selected = 1;
					m_ApplicationSpecification.ColorThema = static_cast<AppColorTheme>(selected);
					SetImGuiTheme();
				}
				if (ImGui::Selectable("Classic", selected == 2))
				{
					selected = 2;
					m_ApplicationSpecification.ColorThema = static_cast<AppColorTheme>(selected);
					SetImGuiTheme();
				}
				if (ImGui::Selectable("Light", selected == 3))
				{
					selected = 3;
					m_ApplicationSpecification.ColorThema = static_cast<AppColorTheme>(selected);
					SetImGuiTheme();
				}
				ImGui::EndMenu();
			}

			// Font
			if (ImGui::BeginMenu("Font"))
			{
				if (ImGui::DragFloat(" Font size ", &m_ApplicationSpecification.FontSize, 0.5f, 9.0f, 50.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					m_FontChanged = true;
				}

				ImGui::Separator();
				int selected = static_cast<int>(m_ApplicationSpecification.Font);
				if (ImGui::Selectable("Roboto", selected == 1))
				{
					selected = 1;
					m_ApplicationSpecification.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
				}
				if (ImGui::Selectable("OpenSans", selected == 2))
				{
					selected = 2;
					m_ApplicationSpecification.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
				}
				if (ImGui::Selectable("Oswald", selected == 3))
				{
					selected = 3;
					m_ApplicationSpecification.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
				}
				if (ImGui::Selectable("Montserrat", selected == 4))
				{
					selected = 4;
					m_ApplicationSpecification.Font = static_cast<AppFont>(selected);
					m_FontChanged = true;
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();
			// Close
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				m_Running = false;
			}
			// ...

			ImGui::EndMenu();
		}
	}

	void Application::LoadFont()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.Fonts->Clear();
		switch (m_ApplicationSpecification.Font)
		{
			case AppFont::Roboto:     m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", m_ApplicationSpecification.FontSize); IM_ASSERT(m_Font != NULL && "Failed to load Roboto font!");  break;
			case AppFont::Montserrat: m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/Montserrat-Regular.ttf", m_ApplicationSpecification.FontSize); break;
			case AppFont::Oswald:     m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Regular.ttf", m_ApplicationSpecification.FontSize); break;
			case AppFont::OpenSans:   m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/Oswald-Regular.ttf", m_ApplicationSpecification.FontSize); break;
			default:                  m_Font = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", m_ApplicationSpecification.FontSize);  break;
		}	
		ImGui_ImplOpenGL3_CreateFontsTexture();
		m_FontChanged = false;
	}

}