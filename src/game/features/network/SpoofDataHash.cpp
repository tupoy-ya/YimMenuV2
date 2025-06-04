#include "core/commands/BoolCommand.hpp"
#include "core/hooking/DetourHook.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/pointers/Pointers.hpp"
#include "types/game_files/CGameDataHash.hpp"

namespace YimMenu::Features
{
	class DumpDataHash : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto log = LOG(VERBOSE);
			log << "DLC Hash: " << BaseHook::Get<Hooks::Network::GetDLCHash, DetourHook<decltype(&Hooks::Network::GetDLCHash)>>()->Original()(*Pointers.DLCManager, 0) << "\n";
			if (auto hashes = Pointers.GameDataHash)
			{
				log << "validHashes = {" << "\n";
				for (int i = 0; i < hashes->m_Data.size(); i++)
					log << hashes->m_Data[i].getData() << ",\n";
				log << "};";
			}
		}
	};
	static DumpDataHash _DumpDataHash{"dumpdatahash", "Dump Data Hash", "Dumps the current data hash into the console"};

	class SpoofDataHash : public BoolCommand
	{
		using BoolCommand::BoolCommand;

		std::array<std::uint32_t, 16> origHashes;

		virtual void OnEnable() override
		{
			constexpr std::array<std::uint32_t, 16> validHashes = {
				2205510306,
				988,
				3512952254,
				472,
				0,
				0,
				2930305905,
				0,
				0,
				1731098795,
				234493012,
				18616,
				1540917665,
				0,
				2624276963,
				200299391,
			};
			if (auto hashes = Pointers.GameDataHash)
			{
				for (int i = 0; i < hashes->m_Data.size(); i++)
					origHashes[i] = hashes->m_Data[i];

				for (int i = 0; i < validHashes.size(); i++)
					hashes->m_Data[i] = validHashes[i];
			}
		}

		virtual void OnDisable() override
		{
			if (auto hashes = Pointers.GameDataHash)
			{
				for (int i = 0; i < origHashes.size(); i++)
					hashes->m_Data[i] = origHashes[i];
			}
		}
	};

	static SpoofDataHash _SpoofDataHash{"spoofdatahash", "Spoof Data Hash", "Allows you to join players with rpf mods (or a half installed game)."};
}

namespace YimMenu::Hooks
{
	uint32_t Network::GetDLCHash(void* manager, uint32_t seed)
	{
		if (YimMenu::Features::_SpoofDataHash.GetState())
			return 612875163;

		return BaseHook::Get<Network::GetDLCHash, DetourHook<decltype(&Network::GetDLCHash)>>()->Original()(manager, seed);
	}
}