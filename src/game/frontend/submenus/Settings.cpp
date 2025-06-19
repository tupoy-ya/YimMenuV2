#include "Settings.hpp"

#include "core/commands/Commands.hpp"
#include "core/commands/HotkeySystem.hpp"
#include "core/commands/LoopedCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/frontend/items/Items.hpp"

namespace YimMenu::Submenus
{
	// TODO: refactor this
	static void Hotkeys()
	{
		ImGui::BulletText("Hold the button with the command name and enter a keystroke to change its hotkey");
		ImGui::BulletText("If a command has an existing hotkey, clicking the button will remove it");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		
		// this assumes we can't add new commands in runtime, but a lot of other subsystems assume that too
		static std::map<std::string, CommandLink*> sortedCommands;
		static bool commandsSorted = []() {
			for (auto& [hash, command] : Commands::GetCommands())
			{
				if (auto it = g_HotkeySystem.m_CommandHotkeys.find(hash); it != g_HotkeySystem.m_CommandHotkeys.end())
					sortedCommands.emplace(command->GetLabel(), &it->second);
			}
			return true;
		}();

		HotkeySystem::SetBeingModifed(false);

		for (auto& [name, link] : sortedCommands)
		{
			ImGui::PushID(link);

			ImGui::Button(name.data());

			bool active = ImGui::IsItemActive();

			if (active)
			{
				HotkeySystem::SetBeingModifed(true);
				g_HotkeySystem.CreateHotkey(link->m_Chain);
			}

			ImGui::SameLine(250);
			ImGui::BeginGroup();

			if (link->m_Chain.empty())
			{
				if (active)
					ImGui::Text("Press any button...");
				else
					ImGui::Text("No hotkey assigned");
			}
			else
			{
				ImGui::PushItemWidth(35);
				int i = 0;
				for (auto key : link->m_Chain)
				{
					char key_label[32];
					strcpy(key_label, g_HotkeySystem.GetHotkeyLabel(key).data());

					ImGui::PushID(i);
					ImGui::InputText("##keylabel", key_label, 32, ImGuiInputTextFlags_ReadOnly);
					if (ImGui::IsItemClicked())
						std::erase_if(link->m_Chain, [key](int j) {
							return j == key;
						});
					ImGui::PopID();

					i++;
					ImGui::SameLine();
				}
				ImGui::PopItemWidth();

				ImGui::SameLine();
				if (ImGui::Button("Clear"))
				{
					link->m_Chain.clear();
				}
			}

			ImGui::EndGroup();

			ImGui::Spacing();

			ImGui::PopID();
		}
	};

	Settings::Settings() :
	#define ICON_FA_GEARS "\xef\x80\x93"
	    Submenu::Submenu("Settings", ICON_FA_GEARS)
	{
		auto hotkeys = std::make_shared<Category>("Hotkeys");
		auto gui = std::make_shared<Category>("GUI");
		auto playerEsp = std::make_shared<Group>("Player ESP", 10);
		auto pedEsp = std::make_shared<Group>("Ped ESP", 10);
		auto objectEsp = std::make_shared<Group>("Object ESP");
		auto overlay = std::make_shared<Group>("Overlay");
		auto chat = std::make_shared<Group>("Chat");

		hotkeys->AddItem(std::make_shared<ImGuiItem>(Hotkeys));

		// Players
		playerEsp->AddItem(std::make_shared<BoolCommandItem>("espdrawplayers"_J));
		playerEsp->AddItem(std::make_shared<ConditionalItem>("espdrawplayers"_J, std::make_shared<BoolCommandItem>("espdrawdeadplayers"_J)));

		playerEsp->AddItem(std::make_shared<ConditionalItem>("espdrawplayers"_J, std::make_shared<BoolCommandItem>("espnameplayers"_J, "Player Name")));
		playerEsp->AddItem(std::make_shared<ConditionalItem>("espdrawplayers"_J, std::make_shared<ColorCommandItem>("namecolorplayers"_J)));

		playerEsp->AddItem(std::make_shared<ConditionalItem>("espdrawplayers"_J, std::make_shared<BoolCommandItem>("espdistanceplayers"_J, "Player Distance")));

		playerEsp->AddItem(std::make_shared<ConditionalItem>("espdrawplayers"_J, std::make_shared<BoolCommandItem>("espskeletonplayers"_J, "Player Skeleton")));
		playerEsp->AddItem(std::make_shared<ConditionalItem>("espdrawplayers"_J, std::make_shared<ColorCommandItem>("skeletoncolorplayers"_J)));

		// Peds
		pedEsp->AddItem(std::make_shared<BoolCommandItem>("espdrawpeds"_J));
		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<BoolCommandItem>("espdrawdeadpeds"_J)));

		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<BoolCommandItem>("espmodelspeds"_J, "Ped Hashes")));
		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<ColorCommandItem>("hashcolorpeds"_J)));

		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<BoolCommandItem>("espnetinfopeds"_J, "Ped Net Info")));
		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<BoolCommandItem>("espscriptinfopeds"_J, "Ped Script Info")));

		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<BoolCommandItem>("espdistancepeds"_J, "Ped Distance")));

		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<BoolCommandItem>("espskeletonpeds"_J, "Ped Skeleton")));
		pedEsp->AddItem(std::make_shared<ConditionalItem>("espdrawpeds"_J, std::make_shared<ColorCommandItem>("skeletoncolorpeds"_J)));

		objectEsp->AddItem(std::make_shared<BoolCommandItem>("espdrawobjects"_J));
		objectEsp->AddItem(std::make_shared<ConditionalItem>("espdrawobjects"_J, std::make_shared<ColorCommandItem>("hashcolorobjects"_J)));
		objectEsp->AddItem(std::make_shared<ConditionalItem>("espdrawobjects"_J, std::make_shared<BoolCommandItem>("espnetinfoobjects"_J, "Object Net Info")));
		objectEsp->AddItem(std::make_shared<ConditionalItem>("espdrawobjects"_J, std::make_shared<BoolCommandItem>("espscriptinfoobjects"_J, "Object Script Info")));

		objectEsp->AddItem(std::make_shared<ConditionalItem>("espdrawobjects"_J, std::make_shared<BoolCommandItem>("espdistanceobjects"_J, "Object Distance")));


		overlay->AddItem(std::make_shared<BoolCommandItem>("overlay"_J));
		overlay->AddItem(std::make_shared<ConditionalItem>("overlay"_J, std::make_shared<BoolCommandItem>("overlayfps"_J)));

		chat->AddItem(std::make_shared<BoolCommandItem>("clearchat"_J));

		gui->AddItem(playerEsp);
		gui->AddItem(pedEsp);
		gui->AddItem(objectEsp);
		gui->AddItem(overlay);
		gui->AddItem(chat);

		AddCategory(std::move(hotkeys));
		AddCategory(std::move(gui));
	}
}