#pragma once

#include "Events/Event.h"

#include "Core/Base.h"

struct GLFWwindow;

namespace Luha {

	using EventCallbackFn = std::function<void(Event&)>;
	struct ApplicationSpecification;
	
	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool Vsync;
		EventCallbackFn EventCallback;
	};

	class Window
	{
	public:
		
		void SetEventCallback(const EventCallbackFn& callback);

		Window(const Luha::ApplicationSpecification& sp);
		~Window();

		void OnUpdate();
		void Shutdown();

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }
		bool IsMaximized() const;

		GLFWwindow* GetNativeWindow() { return m_Window; }
		WindowData GetWindowData() { return m_Data; }

		bool IsVSync() const { return m_Data.Vsync; }
		void SetVSync(bool enabled);

	private:
		GLFWwindow* m_Window;

		
		WindowData m_Data; 
		bool m_Resizeable;
	};

}