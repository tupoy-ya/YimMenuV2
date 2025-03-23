#include "Toxic.hpp"

namespace YimMenu::Submenus
{
	std::shared_ptr<Category> BuildToxicMenu()
	{
		auto menu = std::make_shared<Category>("Toxic");

		auto general = std::make_shared<Group>("Damage", 1);
		general->AddItem(std::make_shared<PlayerCommandItem>("kill"_J));
		general->AddItem(std::make_shared<PlayerCommandItem>("killexploit"_J));
		general->AddItem(std::make_shared<PlayerCommandItem>("explode"_J));

		menu->AddItem(general);

		return menu;
	}
}