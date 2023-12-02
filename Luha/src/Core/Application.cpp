#include "lhpch.h"
#include "Application.h"

#include "glad/glad.h"

namespace Luha {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
	{
		Log::Init();

		s_Instance = this;
		m_Window = CreateScope<Window>(spec);
		m_Window->SetEventCallback(LH_BIND_EVENT_FN(Application::OnEvent));

	}

	Application::~Application()
	{
		Close();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			if (!m_Minimized)
			{
				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

			}
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(LH_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(LH_BIND_EVENT_FN(Application::OnWindowResize));

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		return false;
	}

}