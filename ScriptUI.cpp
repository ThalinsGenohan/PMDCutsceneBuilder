//ScriptUI.cpp - Defines the functions of the "ScriptUI" class

#include "ScriptUI.h"

#include <iostream>
#include <sstream>

#include <SFML/Window.hpp>

// BUTTON
ScriptUI::Button::Button(ButtonType type, sf::Vector2f pos, sf::Vector2f size, std::string filename)
{
	this->type = type;
	this->pos = pos;
	this->size = size;
	rect.left = pos.x;
	rect.top = pos.y;
	rect.width = size.x;
	rect.height = size.y;
	shape.setPosition(pos);
	shape.setSize(size);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(-1.f);
	if (filename != "")
	{
		std::ostringstream oss;
		oss << "assets\\" << filename;
		texture.loadFromFile(oss.str());
		shape.setTexture(&texture);
	}
}
bool ScriptUI::Button::hover(sf::Vector2f m)
{
	if (rect.contains(m))
	{
		shape.setFillColor(sf::Color::Red);
		return true;
	}
	shape.setFillColor(sf::Color::White);
	return false;
}
void ScriptUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
}


// COMMAND
ScriptUI::Command::Command(int ID, CommandType type)
{
	this->ID = ID;
	this->type = type;
}
void ScriptUI::Command::edit()
{
	switch (type)
	{
	case PORTRAIT:
		
		break;
	case DIALOGUE:
		
		break;
	default: 
		break;
	}
}


// COMMANDLIST
ScriptUI::CommandList::CommandList(): selected(0)
{
}
void ScriptUI::CommandList::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	for (auto l = 0; l < int(options.size()); l++)
	{
		target.draw(options[l]);
	}
	target.draw(scrollbar);
}


// COMMANDTEXT
ScriptUI::CommandList::CommandText::CommandText()
{
	
}
void ScriptUI::CommandList::CommandText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(text, states);
}


// SCROLLBAR
ScriptUI::CommandList::ScrollBar::ScrollBar() :
	scrollUp(Button::SCROLLUPBUTTON, { 480.f, 62.5f }, { 20.f, 20.f }, "scrollup.png"),
	scrollDown(Button::SCROLLDOWNBUTTON, { 480.f, 455.f }, { 20.f, 20.f }, "scrolldown.png"),
	thumb(Button::NOBUTTON, { 0,0 }, { 0,0 })
{
}
void ScriptUI::CommandList::ScrollBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(thumb, states);
	target.draw(scrollUp, states);
	target.draw(scrollDown, states);
}



// SCRIPTUI
ScriptUI::ScriptUI()
{
	// ReSharper disable CppNonReclaimedResourceAcquisition

	buttons.push_back(*new Button(Button::NEWBUTTON, { 0.f, 0.f }, { 100.f, 62.5f }, "new.png"));
	buttons.push_back(*new Button(Button::OPENBUTTON, { 100.f, 0.f }, { 100.f, 62.5f }, "open.png"));
	buttons.push_back(*new Button(Button::SAVEBUTTON, { 200.f, 0.f }, { 100.f, 62.5f }, "save.png"));
	buttons.push_back(*new Button(Button::SAVEASBUTTON, { 300.f, 0.f }, { 100.f, 62.5f }, "saveas.png"));
	buttons.push_back(*new Button(Button::CONFIGBUTTON, { 400.f, 0.f }, { 100.f, 62.5f }, "config.png"));

	buttons.push_back(*new Button(Button::ADDBUTTON, { 0.f, 475.f }, { 100.f, 62.5f }, "add.png"));
	buttons.push_back(*new Button(Button::REMOVEBUTTON, { 100.f, 475.f }, { 100.f, 62.5f }, "remove.png"));
	buttons.push_back(*new Button(Button::EDITBUTTON, { 200.f, 475.f }, { 100.f, 62.5f }, "edit.png"));
	buttons.push_back(*new Button(Button::UPBUTTON, { 300.f, 475.f }, { 100.f, 62.5f }, "moveup.png"));
	buttons.push_back(*new Button(Button::DOWNBUTTON, { 400.f, 475.f }, { 100.f, 62.5f }, "movedown.png"));

	buttons.push_back(*new Button(Button::RUNBUTTON, { 0.f, 537.5f }, { 500.f, 62.5f }, "run.png"));


	
}
void ScriptUI::run()
{
	window.create(sf::VideoMode(500, 600), "Scripting Engine Test Pre-Alpha 0.1a", sf::Style::Close | sf::Style::Titlebar);

	sf::Event sEvent;
	auto running = true;

	auto selected = false;
	int selection;

	auto comAct = Command::NOCOMMAND;

	while(running)
	{
		if (window.hasFocus())
		{
			for (auto l = 0; l < int(buttons.size()); l++)
			{
				buttons[l].hover(sf::Vector2f(sf::Mouse::getPosition(window)));
				commandList.scrollbar.scrollUp.hover(sf::Vector2f(sf::Mouse::getPosition(window)));
				commandList.scrollbar.scrollDown.hover(sf::Vector2f(sf::Mouse::getPosition(window)));
				commandList.scrollbar.thumb.hover(sf::Vector2f(sf::Mouse::getPosition(window)));
			}
			if (!selected)
			{
				for (auto l = 6; l < 10; l++)
				{
					buttons[l].shape.setFillColor({ 100, 100, 100 });
				}
			}

			while(window.pollEvent(sEvent))
			{
				switch(sEvent.type)
				{
				case sf::Event::MouseButtonPressed:
					switch (handleClick({ sEvent.mouseButton.x, sEvent.mouseButton.y }))
					{
					case Button::NEWBUTTON:
						
						break;
					case Button::OPENBUTTON:
						
						break;
					case Button::SAVEBUTTON:
						
						break;
					case Button::SAVEASBUTTON:
						
						break;
					case Button::CONFIGBUTTON:
						
						break;
					case Button::ADDBUTTON:
						comAct = add.run();
						if (comAct != Command::NOCOMMAND)
						{
							edit.run(int(commands.size()), comAct);
						}
						break;
					case Button::REMOVEBUTTON:
						if (selected)
						{
							
						}
						break;
					case Button::EDITBUTTON:
						if (selected)
						{

						}
						break;
					case Button::UPBUTTON:
						if (selected)
						{

						}
						break;
					case Button::DOWNBUTTON:
						if (selected)
						{

						}
						break;
					case Button::RUNBUTTON:
						
						break;
					case Button::SCROLLUPBUTTON:
						
						break;
					case Button::SCROLLDOWNBUTTON:
						
						break;
					default:
						break;
					}
					break;

				case sf::Event::Closed:
					running = false;
					break;
				default:
					break;
				}
			}
		}
		while(window.pollEvent(sEvent))
		{
			if (sEvent.type == sf::Event::Closed)
			{
				running = false;
			}
		}

		window.clear();
		window.draw(commandList);
		for (auto l = 0; l < int(buttons.size()); l++)
		{
			window.draw(buttons[l]);
		}
		window.display();
	}
}
ScriptUI::Button::ButtonType ScriptUI::handleClick(sf::Vector2i pos)
{
	for (auto l = 0; l < int(buttons.size()); l++)
	{
		if (buttons[l].rect.contains(float(pos.x), float(pos.y)))
		{
			return buttons[l].type;
		}
	}
	return Button::NOBUTTON;
}


// ADD
ScriptUI::Add::Add()
{
	buttons.push_back(*new Button(Button::ADDPORTRAIT, {0.f, 0.f}, {125.f, 60.f}, "addportrait.png"));
}
ScriptUI::Command::CommandType ScriptUI::Add::run()
{
	window.create(sf::VideoMode(250, 300), "Add Command", sf::Style::Close | sf::Style::Titlebar);

	sf::Event aEvent;
	auto running = true;

	auto r = Command::NOCOMMAND;

	while (running)
	{
		if (window.hasFocus())
		{
			for (auto l = 0; l < int(buttons.size()); l++)
			{
				buttons[l].hover(sf::Vector2f(sf::Mouse::getPosition(window)));
			}

			while (window.pollEvent(aEvent))
			{
				switch (aEvent.type)
				{
				case sf::Event::MouseButtonPressed:
					switch (handleClick({ aEvent.mouseButton.x, aEvent.mouseButton.y }))
					{
					case Button::ADDPORTRAIT:
						running = false;
						r = Command::PORTRAIT;
						break;

					default:
						break;
					}
					break;

				case sf::Event::Closed:
					running = false;
					r = Command::NOCOMMAND;
				default:
					break;
				}
			}
		}

		window.clear();
		for (auto l = 0; l < int(buttons.size()); l++)
		{
			window.draw(buttons[l]);
		}
		window.display();
	}

	window.close();


	return r;
}
ScriptUI::Button::ButtonType ScriptUI::Add::handleClick(sf::Vector2i pos)
{
	for (auto l = 0; l < int(buttons.size()); l++)
	{
		if (buttons[l].rect.contains(float(pos.x), float(pos.y)))
		{
			return buttons[l].type;
		}
	}
	return Button::NOBUTTON;
}


// EDIT
ScriptUI::Edit::Edit()
{
	
}
ScriptUI::Command ScriptUI::Edit::run(int ID, Command::CommandType type)
{
	sf::Event eEvent;

	auto running = true;

	switch (type)
	{
	case Command::PORTRAIT:
		window.create(sf::VideoMode(400, 300), "Edit Command", sf::Style::Close | sf::Style::Titlebar);
		break;
	case Command::DIALOGUE:

		break;

	default:
		return *new Command(0, Command::NOCOMMAND);
	}


	while (running)
	{
		while (window.pollEvent(eEvent))
		{
			switch (eEvent.type)
			{



			case sf::Event::Closed:
				running = false;
				break;
			default:
				break;
			}
		}


		window.clear();

		window.display();
	}


	window.close();
	return *new Command(0, Command::NOCOMMAND);
}
ScriptUI::Command ScriptUI::Edit::run(int ID, Command com)
{
	switch (com.type)
	{
	case Command::PORTRAIT:
		
		break;
	case Command::DIALOGUE:
		
		break;

	default:
		return *new Command(0, Command::NOCOMMAND);
	}

	return *new Command(0, Command::NOCOMMAND);
}