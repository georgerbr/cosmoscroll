#include "KeyboardMenu.hpp"
#include "scenes/ConfigButton.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


KeyboardMenu::KeyboardMenu()
{
	SetTitle(_t("menu.keyboard.title"));

	gui::FormLayout form(100, 100);
	form.SetSpacing(30, 5);
	AddRow(form, Input::MOVE_UP,      &but_up_);
	AddRow(form, Input::MOVE_DOWN,    &but_down_);
	AddRow(form, Input::MOVE_LEFT,    &but_left_);
	AddRow(form, Input::MOVE_RIGHT,   &but_right_);
	AddRow(form, Input::USE_WEAPON_1, &but_weapon1_);
	AddRow(form, Input::USE_WEAPON_2, &but_weapon2_);
	AddRow(form, Input::USE_COOLER,   &but_cooler_);
	AddRow(form, Input::USE_MISSILE,  &but_missile_);

	gui::Button* b = new CosmoButton(this, _t("menu.back"));
	b->SetPosition(210, 410);
	b->SetCallbackID(9000);
}


void KeyboardMenu::OnFocus()
{
	BaseMenu::OnFocus();
	but_up_->SetText(GetKeyLabel(Input::MOVE_UP));
	but_down_->SetText(GetKeyLabel(Input::MOVE_DOWN));
	but_left_->SetText(GetKeyLabel(Input::MOVE_LEFT));
	but_right_->SetText(GetKeyLabel(Input::MOVE_RIGHT));
	but_weapon1_->SetText(GetKeyLabel(Input::USE_WEAPON_1));
	but_weapon2_->SetText(GetKeyLabel(Input::USE_WEAPON_2));
	but_cooler_->SetText(GetKeyLabel(Input::USE_COOLER));
	but_missile_->SetText(GetKeyLabel(Input::USE_MISSILE));
}



void KeyboardMenu::EventCallback(int id)
{
	if (id == 9000)
	{
		Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
	}
	else
	{
		// Input::Action enumerations are used as menu ids
		Input& input = Input::GetInstance();
		input.AskUserInput(Input::KEYBOARD, (Input::Action) id);
		OnFocus();
	}
}


void KeyboardMenu::AddRow(gui::FormLayout& form, Input::Action action, gui::Button** button)
{
	*button = new ConfigButton(this, GetKeyLabel(action));
	(*button)->SetCallbackID(action);

	form.AddRow(Input::ActionToString(action), *button);
}


const char* KeyboardMenu::GetKeyLabel(Input::Action action) const
{
	return Input::KeyToString(Input::GetInstance().GetKeyboardBind(action));
}

