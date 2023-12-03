#pragma once

#include "Events/Event.h"
#include "Base.h"

struct GLFWwindow;

namespace Luha {

	struct ApplicationSpecification;

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		void SetEventCallback(const EventCallbackFn& callback);

		Window(const Luha::ApplicationSpecification& sp);
		~Window();

		void OnUpdate();
		void Shutdown();

		GLFWwindow* GetNativeWindow() { return m_Window; }

	private:
		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }
		bool IsVSync() const { return m_Data.Vsync; }

		void SetVSync(bool enabled);

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool Vsync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data; 
		bool m_Resizeable;
	};

}