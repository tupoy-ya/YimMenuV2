#include "core/commands/BoolCommand.hpp"
#include "core/hooking/DetourHook.hpp"
#include "game/hooks/Hooks.hpp"
#include "game/pointers/Pointers.hpp"
#include "types/game_files/CGameDataHash.hpp"
#include "types/network/rlSessionInfo.hpp"

namespace YimMenu::Features
{
	class LogCurrentSessionInfo : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			char buf[0x100]{};
			Pointers.EncodeSessionInfo((rage::rlSessionInfo*)((uint64_t)*Pointers.NetworkSession+0x1E750), buf, 0xA9, nullptr);
			LOG(VERBOSE) << buf;
		}
	};
	static LogCurrentSessionInfo _LogCurrentSessionInfo{"logcurrentsessioninfo", "Log Current Session Info", "Dumps the current data hash into the console"};
}
