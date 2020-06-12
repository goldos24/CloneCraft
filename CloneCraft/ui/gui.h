#pragma once
#include "ui.h"

namespace gui
{
	struct BackgroundResource
	{
		BackgroundResource();
		BackgroundResource(sf::Color color);
		BackgroundResource(sf::Image image);

		bool useImage = false;
		sf::Image backgroundImage;
		sf::Color backgroundColor;
	};

	struct Gui
	{
		Gui(std::string name);
		Gui(std::string name, BackgroundResource background);
		Gui(std::string name, sf::Color backgroundColor);

		void draw(sf::RenderWindow& window);
		void addElement(ui::UIElement* elem);

		std::string guiName;
		bool drawBackground = true;
		std::vector<ui::UIElement*> uiElements;
		BackgroundResource background;
	};

	struct GuiManager
	{
		GuiManager();
		
		void addGui(Gui* gui);
		void setGuiByName(std::string name);
		bool isGuiWithNameActive(std::string name);
		bool isGuiSet();
		void setNoGui();
		gui::Gui* findGuiByName(std::string name);
		void addUIElementToGuiWithName(ui::UIElement* uiElement, std::string guiName);
		void addButtonToGuiWithName(ui::Button* button, std::string guiName);
		void addTextFieldToGuiWithName(ui::TextField* textField, std::string guiName);
		void drawCurrentGuiToWindow(sf::RenderWindow& window);

		ui::TextFieldManager textFieldManager;
		ui::ButtonManager buttonManager;
		std::vector<Gui*> guis;
		Gui* currentGui;
	};
}