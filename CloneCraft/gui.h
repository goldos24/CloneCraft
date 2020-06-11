#pragma once
#include "ui.h"

namespace gui
{

	struct BackgroundResource
	{
		BackgroundResource(sf::Color color = sf::Color::Black) :
			backgroundColor(color)
		{ }

		BackgroundResource(sf::Image image) :
			backgroundImage(image)
		{
			this->useImage = true;
		}

		bool useImage = false;
		sf::Image backgroundImage;
		sf::Color backgroundColor;
	};

	struct Gui
	{
		Gui(std::string name) : guiName(name)
		{
			this->drawBackground = false;
		}

		Gui(std::string name, BackgroundResource background) : guiName(name)
		{
			this->background = background;
		}

		Gui(std::string name, sf::Color backgroundColor) : guiName(name)
		{
			this->background = BackgroundResource(backgroundColor);
		}

		void draw(sf::RenderWindow& window)
		{
			window.pushGLStates();
			
			if (this->drawBackground) window.clear(this->background.backgroundColor);

			if (this->background.useImage && this->drawBackground)
			{
				sf::Texture backgroundTexture;
				backgroundTexture.loadFromImage(this->background.backgroundImage);
				sf::Sprite bgSprite(backgroundTexture);
				window.draw(bgSprite);
			}

			for (ui::UIElement* element : uiElements)
			{
				if (element != nullptr)
					element->drawToWindow(window);
			}

			ui::Text(this->guiName, this->guiName, ui::fonts::dos, sf::Color::Red, 1, 1, 15).drawToWindow(window);

			window.popGLStates();
		}

		void addElement(ui::UIElement* elem)
		{
			this->uiElements.push_back(elem);
		}

		std::string guiName;
		bool drawBackground = true;
		std::vector<ui::UIElement*> uiElements;
		BackgroundResource background;
	};

	struct GuiManager
	{
		GuiManager() 
		{ 
			this->currentGui = nullptr;
		}

		void addGui(Gui* gui)
		{
			if (this->findGuiByName(gui->guiName) != nullptr) return;
			this->guis.push_back(gui);
		}

		void setGuiByName(std::string name)
		{
			this->textFieldManager.setFocusForAll(false);
			Gui* foundGui = this->findGuiByName(name);
			if (foundGui != nullptr)
			{
				this->currentGui = foundGui;
			}
		}

		bool isGuiWithNameActive(std::string name)
		{
			if (this->currentGui != nullptr) return this->currentGui->guiName == name;
			return false;
		}

		bool isGuiSet()
		{
			return this->currentGui != nullptr;
		}

		void setNoGui()
		{
			this->textFieldManager.setFocusForAll(false);
			this->currentGui = nullptr;
		}

		gui::Gui* findGuiByName(std::string name)
		{
			for (gui::Gui* guiPtr : guis)
			{
				if (guiPtr != nullptr)
					if (guiPtr->guiName == name) return guiPtr;
			}

			return nullptr;
		}

		void addUIElementToGuiWithName(ui::UIElement* uiElement, std::string guiName)
		{
			gui::Gui* gui = this->findGuiByName(guiName);
			if (gui != nullptr)
			{
				gui->addElement(uiElement);
			}
		}

		void addButtonToGuiWithName(ui::Button* button, std::string guiName)
		{
			gui::Gui* gui = this->findGuiByName(guiName);
			if (gui != nullptr)
			{
				gui->addElement(button);
				this->buttonManager.addButton(button);
			}
		}

		void addTextFieldToGuiWithName(ui::TextField* textField, std::string guiName)
		{
			gui::Gui* gui = this->findGuiByName(guiName);
			if (gui != nullptr)
			{
				gui->addElement(textField);
				this->textFieldManager.addTextField(textField);
			}
		}

		void drawCurrentGuiToWindow(sf::RenderWindow& window)
		{
			if (this->currentGui != nullptr) this->currentGui->draw(window);
		}

		ui::TextFieldManager textFieldManager;
		ui::ButtonManager buttonManager;
		std::vector<Gui*> guis;
		Gui* currentGui;
	};
}