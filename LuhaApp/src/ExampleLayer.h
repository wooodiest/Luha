#pragma once

#include "Luha.h"

namespace ExampleApp {

	class ExampleLayer : public Luha::Layer
	{
	public:
		ExampleLayer();
		virtual ~ExampleLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Luha::Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnMainMenuRender() override;
		virtual void OnEvent(Luha::Event& event) override;

	};

}