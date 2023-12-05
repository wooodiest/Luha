#include "lhpch.h"
#include "Time.h"

#include "Core/Application.h"

namespace Luha {

	void Clock::OnUpdate()
	{
		if (!m_Active)
			return;

		m_CurrentTime += Application::Get().GetDeltaTime();
		if (m_CurrentTime >= m_Time)
		{
			m_Active = false;
			m_Callback();
		}

	}
	void RepeatClock::OnUpdate()
	{
		if (!m_Active)
			return;

		m_CurrentTime += Application::Get().GetDeltaTime();
		if (m_CurrentTime >= m_Time)
		{
			m_CurrentTime -= m_Time;
			m_CallCount++;
			m_Callback();
		}
	}
}