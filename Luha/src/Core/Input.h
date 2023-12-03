#pragma once

#include "Core/Base.h"
#include "Core/Codes.h"

namespace Luha {

	class Input
	{
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMousePositionX();
		static float GetMousePositionY();

	private:
		Input() = default;
		
	};

}