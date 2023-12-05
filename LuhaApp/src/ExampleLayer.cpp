#include "ExampleLayer.h"

#include "imgui.h"
#include "implot.h"

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
			LH_INFO("Key A is pressed, random number: {0}", Luha::Random::Int());

		if (Luha::Input::IsMouseButtonPressed(Luha::MouseCode::ButtonRight))
			LH_INFO("Left mouse button is clicked: ({0}, {1})", Luha::Input::GetMousePositionX(), Luha::Input::GetMousePositionY());
	
		static Luha::Timestep time = 0.0f; 
		static auto clock1 = Luha::Clock(Luha::Timestep(5.0f), []() { LH_INFO("It's been ~ 5s: {0}s", (float)time); });
		clock1.OnUpdate(); 
		time += ts;

		static auto clock2 = Luha::RepeatClock(Luha::Timestep(3.0f), false, []() { LH_INFO("Hi"); });
		clock2.OnUpdate();
	}

	void ExampleLayer::OnImGuiRender()
	{
		LH_PROFILE_FUNCTION();

		ImGui::ShowDemoWindow();
		ImPlot::ShowDemoWindow();

		ImGui::Begin("Example Window");
		ImGui::Text("Ala ma kota");
		ImGui::Text("App is live for %.1fs", Luha::Application::Get().GetTime());
		ImGui::Text("Delta time: %.1fms", Luha::Application::Get().GetDeltaTime().GetMilliseconds());
		ImGui::Text("Total frames: %d", Luha::Application::Get().GetWindow().GetFrameCount());
		ImGui::End();

	}

	void ExampleLayer::OnMainMenuRender()
	{
		LH_PROFILE_FUNCTION();

		if (ImGui::BeginMenu("Example"))
		{
			ImGui::Text("JD2000");

			ImGui::EndMenu();
		}
	}

	void ExampleLayer::OnEvent(Luha::Event& event)
	{
		if (event.GetEventType() == Luha::EventType::WindowResize)
		{
			Luha::ScopedTimer("Window resized", Luha::TimeUnit::ms);

			auto ev = (Luha::WindowResizeEvent&)event;
			LH_INFO("Window Resized: ({0}, {1})", ev.GetWidth(), ev.GetHeight());
		}
	}
}