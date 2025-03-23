#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"

namespace YimMenu::Features
{
	class NoRagdoll : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			if (!Self::GetPed())
				return;
			
			Self::GetPed().SetRagdoll(false);
		}

		virtual void OnDisable() override
		{
			if (!Self::GetPed())
				return;

			Self::GetPed().SetRagdoll(true);
		}
	};

	static NoRagdoll _NoRagdoll{"noragdoll", "No Ragdoll", "Prevents your ped from ragdolling"};
}