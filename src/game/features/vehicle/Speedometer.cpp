#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/gta/Natives.hpp"

namespace YimMenu::Features
{
	class Speedometer : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;
		int m_ScaleformHandle{};

		bool EnsureScaleformLoaded()
		{
			if (m_ScaleformHandle && GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(m_ScaleformHandle))
				return true;

			if (!m_ScaleformHandle)
				m_ScaleformHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("DRAG_RACE");

			return false;
		}

		int GetVehicleSpeed(Vehicle veh)
		{
			auto speed = veh.GetSpeed();

			if (MISC::SHOULD_USE_METRIC_MEASUREMENTS())
				return speed * 3.6f;
			else
				return speed * 2.23694f;
		}

		virtual void OnTick() override
		{
			auto veh = Self::GetVehicle();
			
			if (!veh || !EnsureScaleformLoaded() || STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
				return;

			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_GEAR");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(veh.GetGear());
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_SPEED");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(GetVehicleSpeed(veh));
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_SPEED_UNITS");
			GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRING");
			HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(MISC::SHOULD_USE_METRIC_MEASUREMENTS() ? "KPH" : "MPH");
			GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_METER_VALUE");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(veh.GetSpeed() / veh.GetMaxSpeed());
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_OUTER_GOAL");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(-1.0f);
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(-1.0f);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_INNER_GOAL");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(-1.0f);
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(-1.0f);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_SCREEN_POSITION");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(1.0f);
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_FLOAT(0.85f);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(m_ScaleformHandle, "SET_IS_DRIFT_RACE");
			GRAPHICS::SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
			GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
			GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(m_ScaleformHandle, 255, 255, 255, 255, 0);
		}

		virtual void OnDisable() override
		{
			GRAPHICS::SET_SCALEFORM_MOVIE_AS_NO_LONGER_NEEDED(&m_ScaleformHandle);
			m_ScaleformHandle = 0;
		}
	};

	static Speedometer _Speedometer{"speedometer", "Speedometer", "Shows a speedometer whenever you are in a vehicle"};
}