#pragma once

#include "Events/Event.h"

#include "Core/Timestep.h"

namespace Luha {

	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(Timestep ts) {}
		virtual void OnRender() {}
		virtual void OnMenuRender() {}
		virtual void OnEvent(Event& event) {}

		virtual void Serialize() {}
		virtual void Deserialize() {}

	};

}
