#include "SavedVehicles.hpp"

#include "core/backend/FiberPool.hpp"
#include "core/frontend/Notifications.hpp"
#include "core/util/Strings.hpp"
#include "game/backend/Self.hpp"
#include "game/backend/SavedVehicles.hpp"
#include "game/gta/Vehicle.hpp"
#include "misc/cpp/imgui_stdlib.h"

namespace YimMenu::Submenus
{
	std::shared_ptr<Category> BuildSavedVehiclesMenu()
	{
		static std::string folder{}, file{};
		static std::vector<std::string> folders{}, files{};
		static char vehicle_file_name_input[64]{};
		static char save_folder[50]{};

		auto persistCar = std::make_shared<Category>("Saved Vehicles");

		persistCar->AddItem(std::make_unique<ImGuiItem>([] {
			static auto drawSaveVehicleButton = [](const char* folderToSaveIn) {
				if (!Self::GetVehicle() || !Self::GetVehicle().IsValid())
					return;

				if (ImGui::Button("Save"))
				{
					std::string yo = vehicle_file_name_input;
					ZeroMemory(vehicle_file_name_input, sizeof(vehicle_file_name_input));

					if (!TrimString(yo).size())
					{
						Notifications::Show("Saved Vehicles", "Filename empty!", NotificationType::Warning);
						return;
					}

					ReplaceString(yo, ".", ""); // so that .. does not throw error by custom file system when it sees say bob..json
					yo += ".json";

					SavedVehicles::Save(folderToSaveIn, yo);
					SavedVehicles::RefreshList(folder, folders, files);
				}
				ImGui::SameLine();
				if (ImGui::Button("Populate Name"))
					FiberPool::Push([] {
						std::string name = Self::GetVehicle().GetFullName();
						strcpy(vehicle_file_name_input, name.c_str());
					});
			};

			if (ImGui::Button("Refresh List"))
				SavedVehicles::RefreshList(folder, folders, files);

			ImGui::SetNextItemWidth(300.f);
			auto folder_display = folder.empty() ? "Root" : folder.c_str();
			if (ImGui::BeginCombo("Folder", folder_display))
			{
				if (ImGui::Selectable("Root", folder == ""))
				{
					folder.clear();
					SavedVehicles::RefreshList(folder, folders, files);
				}

				for (std::string folder_name : folders)
					if (ImGui::Selectable(folder_name.c_str(), folder == folder_name))
					{
						folder = folder_name;
						SavedVehicles::RefreshList(folder, folders, files);
					}

				ImGui::EndCombo();
			}

			static bool open_modal = false;
			static std::string search;

			ImGui::SetNextItemWidth(300);
			if (ImGui::InputTextWithHint("###veh_name", "Search", &search))
				std::transform(search.begin(), search.end(), search.begin(), tolower);

			ImGui::Text("Saved Vehicles");

			static const auto over_30 = (30 * ImGui::GetTextLineHeightWithSpacing() + 2);
			const auto box_height = files.size() <= 30 ? (files.size() * ImGui::GetTextLineHeightWithSpacing() + 2) : over_30;
			ImGui::SetNextItemWidth(250);
			if (ImGui::BeginListBox("##saved_vehs", ImVec2(300, box_height)))
			{
				for (const auto& pair : files)
				{
					std::string pair_lower = pair;
					std::transform(pair_lower.begin(), pair_lower.end(), pair_lower.begin(), tolower);
					if (pair_lower.contains(search))
					{
						auto file_name = pair.c_str();
						if (ImGui::Selectable(file_name, file == pair, ImGuiSelectableFlags_AllowItemOverlap))
						{
							file       = pair;
							open_modal = true;
						}
					}
				}
				ImGui::EndListBox();
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			{
				ImGui::Text("File Name");
				ImGui::SetNextItemWidth(250);
				ImGui::InputText("##vehiclefilename", vehicle_file_name_input, IM_ARRAYSIZE(vehicle_file_name_input));

				if (folder.empty())
				{
					ImGui::Text("Folder Name");
					ImGui::SetNextItemWidth(250);
					ImGui::InputText("##foldername", save_folder, IM_ARRAYSIZE(save_folder));
					drawSaveVehicleButton(save_folder);
				}
				else
					drawSaveVehicleButton(folder.c_str());
			}
			ImGui::EndGroup();

			if (open_modal)
				ImGui::OpenPopup("##spawncarmodel2");
			if (ImGui::BeginPopupModal("##spawncarmodel2", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
			{
				ImGui::Text("Are you sure you want to spawn %s", file.c_str());
				ImGui::Spacing();
				if (ImGui::Button("Yes"))
				{
					SavedVehicles::Load(folder, file);
					open_modal = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("No"))
				{
					open_modal = false;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}));

		return persistCar;
	}
}
