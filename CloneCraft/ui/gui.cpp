#include "gui.h"

// BackgroundResource
gui::BackgroundResource::BackgroundResource() { }

gui::BackgroundResource::BackgroundResource(sf::Image image) : backgroundImage(image) 
{ 
	this->useImage = true;
}

gui::BackgroundResource::BackgroundResource(sf::Color color) : backgroundColor(color) { }

// Gui
gui::Gui::Gui(std::string name) : guiName(name)
{
	this->drawBackground = false;
}

gui::Gui::Gui(std::string name, gui::BackgroundResource background) : guiName(name)
{
	this->background = background;
}

gui::Gui::Gui(std::string name, sf::Color backgroundColor) : guiName(name)
{
	this->background = gui::BackgroundResource(backgroundColor);
}

void gui::Gui::draw(sf::RenderWindow& window)
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

void gui::Gui::addElement(ui::UIElement* elem)
{
	this->uiElements.push_back(elem);
}

// GuiManager
gui::GuiManager::GuiManager()
{
	this->currentGui = nullptr;
}

void gui::GuiManager::addGui(Gui* gui)
{
	if (this->findGuiByName(gui->guiName) != nullptr) return;
	this->guis.push_back(gui);
}

void gui::GuiManager::setGuiByName(std::string name)
{
	this->textFieldManager.setFocusForAll(false);
	Gui* foundGui = this->findGuiByName(name);
	if (foundGui != nullptr)
	{
		this->currentGui = foundGui;
	}
}

bool gui::GuiManager::isGuiWithNameActive(std::string name)
{
	if (this->currentGui != nullptr) return this->currentGui->guiName == name;
	return false;
}

bool gui::GuiManager::isGuiSet()
{
	return this->currentGui != nullptr;
}

void gui::GuiManager::setNoGui()
{
	this->textFieldManager.setFocusForAll(false);
	this->currentGui = nullptr;
}

gui::Gui* gui::GuiManager::findGuiByName(std::string name)
{
	for (gui::Gui* guiPtr : this->guis)
	{
		if (guiPtr != nullptr)
			if (guiPtr->guiName == name) return guiPtr;
	}

	return nullptr;
}

void gui::GuiManager::addUIElementToGuiWithName(ui::UIElement* uiElement, std::string guiName)
{
	gui::Gui* gui = this->findGuiByName(guiName);
	if (gui != nullptr)
	{
		gui->addElement(uiElement);
	}
}

void gui::GuiManager::addButtonToGuiWithName(ui::Button* button, std::string guiName)
{
	gui::Gui* gui = this->findGuiByName(guiName);
	if (gui != nullptr)
	{
		gui->addElement(button);
		this->buttonManager.addButton(button);
	}
}

void gui::GuiManager::addTextFieldToGuiWithName(ui::TextField* textField, std::string guiName)
{
	gui::Gui* gui = this->findGuiByName(guiName);
	if (gui != nullptr)
	{
		gui->addElement(textField);
		this->textFieldManager.addTextField(textField);
	}
}

void gui::GuiManager::drawCurrentGuiToWindow(sf::RenderWindow& window)
{
	if (this->currentGui != nullptr) this->currentGui->draw(window);
}