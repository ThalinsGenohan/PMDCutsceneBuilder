#include "ScriptUI.h"

#include <iostream>
#include <sstream>

#include <SFML/Window.hpp>

//BUTTON

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


//COMMAND

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


//SELECT

ScriptUI::Select::Select(): selected(0)
{
}

void ScriptUI::Select::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	for (auto l = 0; l < int(options.size()); l++)
	{
		target.draw(options[l]);
	}
	target.draw(scrollbar);
}


//SELOPTION

ScriptUI::Select::SelOption::SelOption()
{
	
}

void ScriptUI::Select::SelOption::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(text, states);
}


//SCROLLBAR

ScriptUI::Select::ScrollBar::ScrollBar() :
	scrollUp(Button::SCROLLUPBUTTON, { 480.f, 62.5f }, { 20.f, 20.f }, "scrollup.png"),
	scrollDown(Button::SCROLLDOWNBUTTON, { 480.f, 455.f }, { 20.f, 20.f }, "scrolldown.png"),
	scroll(Button::NOBUTTON, { 0,0 }, { 0,0 })
{
}


void ScriptUI::Select::ScrollBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(scroll, states);
	target.draw(scrollUp, states);
	target.draw(scrollDown, states);
}



//SCRIPTUI

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
				commandList.scrollbar.scroll.hover(sf::Vector2f(sf::Mouse::getPosition(window)));
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


//ADD

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


//EDIT

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


/*

ScriptUI::Button::Button(ButtonType type, sf::Vector2f pos, sf::Vector2f size, std::string filename)
{
	this->type = type;
	rect.left = pos.x;
	rect.top = pos.y;
	rect.width = size.x;
	rect.height = size.y;
	shape.setPosition(pos);
	shape.setSize(size);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(-1.f);
	texture.loadFromFile(filename);
	shape.setTexture(&texture);
}



void ScriptUI::Textbox::Command::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}

void ScriptUI::Textbox::ScrollBar::updateSize(int lines)
{
	barSize = 412.5f / (lines * 18.f / 412.5f);
	shapes[1].setSize({ rects[1].width, barSize });
	rects[1].height = barSize;
}

void ScriptUI::Textbox::ScrollBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto l = 0; l < int(shapes.size()); l++)
	{
		target.draw(shapes[l], states);
	}
}


ScriptUI::Textbox::Command::Command(): type(), num(0)
{
}

ScriptUI::Textbox::Command::Command(CommandType type): num(0)
{
	this->type = type;
}

void ScriptUI::Textbox::Command::add(std::string text)
{

}
void ScriptUI::Textbox::Command::edit()
{
	
}



void ScriptUI::Textbox::updateScroll()
{
	auto lineNum = 1;
	for (auto l = 0; l < int(commands.size()); l++)
	{
	//	lineNum += commands[l].lines.size();
	}
	scrollbar.updateSize(lineNum);
}

void ScriptUI::Textbox::addCommand(Command::CommandType type)
{
	commands.push_back(*new Command(type));
}


void ScriptUI::Textbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape);
	for (auto l = 0; l < int(commands.size()); l++)
	{
		target.draw(commands[l], states);
	}
	target.draw(scrollbar);
}



ScriptUI::ScriptUI()
{
	// ReSharper disable CppNonReclaimedResourceAcquisition
	buttons.push_back(*new Button(Button::NEW, { 0.f, 0.f }, { 100.f, 62.5f }, "new.png"));
	buttons.push_back(*new Button(Button::LOAD, { 100.f, 0.f }, { 100.f, 62.5f }, "load.png"));
	buttons.push_back(*new Button(Button::SAVE, { 200.f, 0.f }, { 100.f, 62.5f }, "save.png"));
	buttons.push_back(*new Button(Button::SAVEAS, { 300.f, 0.f }, { 100.f, 62.5f }, "saveas.png"));
	buttons.push_back(*new Button(Button::CONFIG, { 400.f, 0.f }, {100.f, 62.5f}, "config.png"));

	buttons.push_back(*new Button(Button::ADD, { 0.f, 475.f }, { 100.f, 62.5f }, "add.png"));
	buttons.push_back(*new Button(Button::REMOVE, { 100.f, 475.f }, { 100.f, 62.5f }, "remove.png"));
	buttons.push_back(*new Button(Button::EDIT, { 200.f, 475.f }, { 100.f, 62.5f }, "edit.png"));
	buttons.push_back(*new Button(Button::MOVEUP, { 300.f, 475.f }, { 100.f, 62.5f }, "up.png"));
	buttons.push_back(*new Button(Button::MOVEDOWN, { 400.f, 475.f }, { 100.f, 62.5f }, "down.png"));

	buttons.push_back(*new Button(Button::RUN, { 0.f, 537.5f }, { 500.f, 62.5f }, "run.png"));

	buttons.push_back(*new Button(Button::SCROLLUP, { 480.f, 62.5f }, { 20.f, 20.f }, "scrollup.png"));
	buttons.push_back(*new Button(Button::SCROLLDOWN, { 480.f, 455.f }, { 20.f, 20.f }, "scrolldown.png"));


	textbox.shape.setSize({ 500.f, 412.5f });
	textbox.shape.setPosition(0.f, 62.5);
	textbox.shape.setFillColor(sf::Color::Cyan);

	textbox.scrollbar.rects.push_back(*new sf::Rect<float>({ 480.f, 62.5f }, { 20.f, 412.5f }));
	textbox.scrollbar.rects.push_back(*new sf::Rect<float>({ 480.f, 455.f }, { 20.f, textbox.scrollbar.barSize }));

	textbox.scrollbar.shapes.push_back(*new sf::RectangleShape({ textbox.scrollbar.rects[0].width, textbox.scrollbar.rects[0].height }));
	textbox.scrollbar.shapes[0].setPosition({ 480.f, 62.5f });
	textbox.scrollbar.shapes[0].setOutlineColor(sf::Color::Black);
	textbox.scrollbar.shapes[0].setOutlineThickness(-1.f);

	textbox.scrollbar.shapes.push_back(*new sf::RectangleShape({ textbox.scrollbar.rects[1].width, textbox.scrollbar.rects[1].height }));
	textbox.scrollbar.shapes[1].setPosition({ 480.f, textbox.scrollbar.barPos });
	textbox.scrollbar.shapes[1].setFillColor(sf::Color::Blue);
	textbox.scrollbar.shapes[1].setOutlineColor(sf::Color::Black);
	textbox.scrollbar.shapes[1].setOutlineThickness(-1.f);



	addButtons.push_back(*new Button(Button::ADDPORTRAIT, { 0.f, 0.f }, { 125.f, 60.f }, "addportrait.png"));
	addButtons.push_back(*new Button(Button::ADDDIALOGUE, { 125.f, 0.f }, { 125.f, 60.f }, "adddialogue.png"));
}

void ScriptUI::createWindow()
{
	window.create(sf::VideoMode(500, 600), "Scripting", sf::Style::Close | sf::Style::Titlebar);
}
void ScriptUI::closeWindow()
{
	window.close();
}
void ScriptUI::showWindow()
{
	window.setVisible(true);
}
void ScriptUI::hideWindow()
{
	window.setVisible(false);
}

void ScriptUI::addCommand()
{
	addWindow.create(sf::VideoMode(250, 300), "Add Command", sf::Style::Close | sf::Style::Titlebar);
}
void ScriptUI::editCommand(int selected)
{
	textbox.commands[selected].edit();
}
void ScriptUI::editCommand(Button::ButtonType type)
{
	editWindow.create(sf::VideoMode(400, 300), "Edit Command", sf::Style::Close | sf::Style::Titlebar);
}




void ScriptUI::run()
{
	sf::Event scriptEvent;
	auto running = true;

	while(running)
	{
		if (window.hasFocus())
		{
			for (auto l = 0; l < int(buttons.size()); l++)
			{
				if (buttons[l].rect.contains(sf::Vector2f(sf::Mouse::getPosition(window))))
				{
					buttons[l].shape.setFillColor(sf::Color::Red);
				}
				else
				{
					buttons[l].shape.setFillColor(sf::Color::White);
				}
			}
		}
		if (addWindow.hasFocus())
		{
			for (auto l = 0; l < int(addButtons.size()); l++)
			{
				if (addButtons[l].rect.contains(sf::Vector2f(sf::Mouse::getPosition(addWindow))))
				{
					addButtons[l].shape.setFillColor(sf::Color::Red);
				}
				else
				{
					addButtons[l].shape.setFillColor(sf::Color::White);
				}
			}
		}


		textbox.updateScroll();

		while (window.pollEvent(scriptEvent))
		{
			if (window.hasFocus())
			{
				switch (scriptEvent.type)
				{
				case sf::Event::MouseButtonPressed:
					switch (handleClick({ scriptEvent.mouseButton.x, scriptEvent.mouseButton.y }))
					{
					case Button::ADD:
						addCommand();
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
		while (addWindow.pollEvent(scriptEvent))
		{
			if (addWindow.hasFocus())
			{
				switch (scriptEvent.type)
				{
				case sf::Event::MouseButtonPressed:
					editCommand(handleClick({ scriptEvent.mouseButton.x, scriptEvent.mouseButton.y }));
					break;

				case sf::Event::Closed:
					addWindow.close();
					break;
					
				default:
					break;
				}
			}
		}
		while (editWindow.pollEvent(scriptEvent))
		{
			if (editWindow.hasFocus())
			{
				switch (scriptEvent.type)
				{
				case sf::Event::Closed:
					editWindow.close();
					break;

				default:
					break;
				}
			}
		}
		
		
		window.clear(sf::Color::Black);
		window.draw(textbox);
		for (auto l = 0; l < int(buttons.size()); l++)
		{
			window.draw(buttons[l].shape);
		}
		window.display();

		addWindow.clear(sf::Color::Black);
		for (auto l = 0; l < int(addButtons.size()); l++)
		{
			addWindow.draw(addButtons[l].shape);
		}
		addWindow.display();

		editWindow.clear(sf::Color::Black);
		for (auto l = 0; l < int(editButtons.size()); l++)
		{
			editWindow.draw(editButtons[l].shape);
		}
		editWindow.display();
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


*/
