#include "lhpch.h"
#include "Application.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Timestep.h"

namespace Luha {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
	{
		LH_PROFILE_FUNCTION();

		Log::Init();

		s_Instance = this;
		m_Window = CreateScope<Window>(spec);
		m_Window->SetEventCallback(LH_BIND_EVENT_FN(Application::OnEvent));

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
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Calculate delta time
			float time = GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				LH_PROFILE_SCOPE("LayerStack OnUpdate()");

				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
					layer->OnImGuiRender();
				}
			}
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		LH_PROFILE_FUNCTION();
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

}