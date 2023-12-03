#include "ExampleLayer.h"

namespace ExampleApp {

	ExampleLayer::ExampleLayer()
	{
		LH_PROFILE_FUNCTION();
	}

	ExampleLayer::~ExampleLayer()
	{
		LH_PROFILE_FUNCTION();
	}

	void ExampleLayer::OnAttach()
	{
		LH_PROFILE_FUNCTION();
	}

	void ExampleLayer::OnDetach()
	{
		LH_PROFILE_FUNCTION();
	}

	void ExampleLayer::OnUpdate(Luha::Timestep ts)
	{
		LH_PROFILE_FUNCTION();

		if (Luha::Input::IsKeyPressed(Luha::KeyCode::A))
			LH_INFO("Key A is pressed!");

		if (Luha::Input::IsMouseButtonPressed(Luha::MouseCode::ButtonLeft))
			LH_INFO("Left mouse button is clicked: ({0}, {1})", Luha::Input::GetMousePositionX(), Luha::Input::GetMousePositionY());
	
	}

	void ExampleLayer::OnImGuiRender()
	{
		LH_PROFILE_FUNCTION();
	}

	void ExampleLayer::OnEvent(Luha::Event& event)
	{
		if (event.GetEventType() == Luha::EventType::WindowResize)
		{
			auto ev = (Luha::WindowResizeEvent&)event;
			LH_INFO("Window Resized: ({0}, {1})", ev.GetWidth(), ev.GetHeight());
		}
	}
}