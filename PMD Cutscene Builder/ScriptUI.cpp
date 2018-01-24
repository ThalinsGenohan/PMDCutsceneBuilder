//ScriptUI.cpp - Defines the functions of the "ScriptUI" class

#include "ScriptUI.h"

#include <iostream>
#include <sstream>
#include <filesystem>

#include <Windows.h>

#include <SFML/Window.hpp>

namespace fs = std::experimental::filesystem;

sf::Font font;

void init()
{
	if (!font.loadFromFile("wondermail.ttf"))
	{
		std::cerr << "[ERROR] Unable to load font \"wondermail.ttf\".";
		system("pause");
	}
}

std::wstring s2ws(const std::string& s)
{
	const auto slength = int(s.length()) + 1;
	const auto len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, nullptr, 0);
	const auto buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

/**
*	Creates a new sf::Image and
*	loads it with image data from a resource (.rc) file
*/
sf::Image LoadImageFromResource(std::string name)
{
	const auto rsrcData = FindResource(nullptr, s2ws(name).c_str(), RT_RCDATA);
	if (!rsrcData)
		throw std::runtime_error("Failed to find resource.");

	const auto rsrcDataSize = SizeofResource(nullptr, rsrcData);
	if (rsrcDataSize <= 0)
		throw std::runtime_error("Size of resource is 0.");

	const auto grsrcData = LoadResource(nullptr, rsrcData);
	if (!grsrcData)
		throw std::runtime_error("Failed to load resource.");

	const auto firstByte = LockResource(grsrcData);
	if (!firstByte)
		throw std::runtime_error("Failed to lock resource.");

	sf::Image image;
	if (!image.loadFromMemory(firstByte, rsrcDataSize))
		throw std::runtime_error("Failed to load image from memory.");

	return image;
}



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
		texture.loadFromImage(LoadImageFromResource(filename));
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
ScriptUI::Command::Command(int ID, CommandType type, std::string com)
{
	this->ID = ID;
	this->type = type;
	this->com = com;
}
// ReSharper disable once CppMemberFunctionMayBeConst
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
ScriptUI::CommandList::CommandList() :
	selected(0),
	rect({ 0.f, 62.5f }, { 480.f, 475.f }),
	shape({ 480.f, 475.f })
{
	shape.setPosition(rect.left, rect.top);
}
void ScriptUI::CommandList::handleScroll(float mouse, sf::Vector2f initial)
{
	const auto prevThumb = scrollbar.thumbPos;

	if (scrollbar.thumb.rect.contains(initial))
	{
		scrollbar.updatePos(mouse - initial.y);
	}

	auto scrollRatio = 0.f;

	if (float(commandTexts.size()) * fontHeight - rect.height > 0)
	{
		scrollRatio = (float(commandTexts.size()) * fontHeight + 2.f - rect.height) / (scrollbar.rect.height - scrollbar.thumb.rect.height);
	}

	for (auto l = 0; l < int(commandTexts.size()); l++)
	{
		commandTexts[l].setPosition({ commandTexts[l].text.getPosition().x,
			commandTexts[l].text.getPosition().y +
			scrollRatio *
			(prevThumb - scrollbar.thumbPos) });
	}
}
void ScriptUI::CommandList::addCommand(Command com)
{
	// ReSharper disable once CppNonReclaimedResourceAcquisition
	commandTexts.push_back(*new CommandText(com));

	commandTexts[int(commandTexts.size()) - 1].setPosition({ 0.f, 63.f + 21.f * (float(commandTexts.size()) - 1.f) });
}
void ScriptUI::CommandList::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	for (auto l = 0; l < int(commandTexts.size()); l++)
	{
		target.draw(commandTexts[l], states);
	}
	target.draw(scrollbar);
}


// COMMANDTEXT
ScriptUI::CommandList::CommandText::CommandText(Command com)
{
	text.setFont(font);
	text.setFillColor(sf::Color::Black);
	text.setString(com.com);

	shape.setSize({ 21.f, 475.f });
}
void ScriptUI::CommandList::CommandText::setPosition(sf::Vector2f pos)
{
	text.setPosition(pos);
	shape.setPosition({ pos.x - 6.f, pos.y + 13.f });
	rect.left = pos.x - 6.f + 1.f;
	rect.top = pos.y + 13.f + 1.f;
}
void ScriptUI::CommandList::CommandText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(text, states);
}


// SCROLLBAR
ScriptUI::CommandList::ScrollBar::ScrollBar() :
	rect({480.f, 82.5f}, {20.f, 392.5f}),
	scrollUp(Button::SCROLLUPBUTTON, {480.f, 62.5f}, {20.f, 20.f}, ""),
	scrollDown(Button::SCROLLDOWNBUTTON, {480.f, 455.f}, {20.f, 20.f}, ""),
	thumb(Button::SCROLLTHUMBBUTTON, {480.f, 82.5f}, {20.f, thumbSize}, ""),
	fontHeight(21) {}
void ScriptUI::CommandList::ScrollBar::updateSize(int lines)
{
	if (lines * fontHeight / rect.height >= 1.f)
		thumbSize = rect.height / (lines * fontHeight / rect.height);
	else
		thumbSize = rect.height;
	thumb.rect.height = thumbSize;
	thumb.shape.setSize({ rect.width, thumbSize });
}
void ScriptUI::CommandList::ScrollBar::updatePos(float pos)
{
	if ((pos <= 0 || thumb.rect.top + thumb.rect.height <= scrollDown.rect.top)
		&& (pos >= 0 || thumb.rect.top >= scrollUp.rect.top + scrollUp.rect.height))
	{
		this->thumbPos += pos;

		thumb.pos.y += pos;
		thumb.rect.top += pos;
		thumb.shape.setPosition(thumb.shape.getPosition().x, thumb.shape.getPosition().y + pos);
	}
	if (thumb.rect.top + thumb.rect.height >= scrollDown.rect.top)
	{
		this->thumbPos = scrollDown.rect.top - thumb.rect.height;

		thumb.pos.y = scrollDown.rect.top - thumb.rect.height;
		thumb.rect.top = scrollDown.rect.top - thumb.rect.height;
		thumb.shape.setPosition(thumb.shape.getPosition().x, scrollDown.rect.top - thumb.rect.height);
	}
	if (thumb.rect.top <= scrollUp.rect.top + scrollUp.rect.height)
	{
		this->thumbPos = scrollUp.rect.top + scrollUp.rect.height;

		thumb.pos.y = scrollUp.rect.top + scrollUp.rect.height;
		thumb.rect.top = scrollUp.rect.top + scrollUp.rect.height;
		thumb.shape.setPosition(thumb.shape.getPosition().x, scrollUp.rect.top + scrollUp.rect.height);
	}
}
void ScriptUI::CommandList::ScrollBar::resetPos()
{
	this->thumbPos = rect.top + rect.width;

	thumb.pos.y = rect.top + rect.width;
	thumb.rect.top = rect.top + rect.width;
	thumb.shape.setPosition(thumb.shape.getPosition().x, rect.top + rect.width);
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
	init();

	// ReSharper disable CppNonReclaimedResourceAcquisition

	buttons.push_back(*new Button(Button::NEWBUTTON, { 0.f, 0.f }, { 100.f, 62.5f }, "NEW"));
	buttons.push_back(*new Button(Button::OPENBUTTON, { 100.f, 0.f }, { 100.f, 62.5f }, "OPEN"));
	buttons.push_back(*new Button(Button::SAVEBUTTON, { 200.f, 0.f }, { 100.f, 62.5f }, "SAVE"));
	buttons.push_back(*new Button(Button::SAVEASBUTTON, { 300.f, 0.f }, { 100.f, 62.5f }, "SAVEAS"));
	buttons.push_back(*new Button(Button::CONFIGBUTTON, { 400.f, 0.f }, { 100.f, 62.5f }, "CONFIG"));

	buttons.push_back(*new Button(Button::ADDBUTTON, { 0.f, 475.f }, { 100.f, 62.5f }, "ADD"));
	buttons.push_back(*new Button(Button::REMOVEBUTTON, { 100.f, 475.f }, { 100.f, 62.5f }, "REMOVE"));
	buttons.push_back(*new Button(Button::EDITBUTTON, { 200.f, 475.f }, { 100.f, 62.5f }, "EDIT"));
	buttons.push_back(*new Button(Button::UPBUTTON, { 300.f, 475.f }, { 100.f, 62.5f }, "MOVEUP"));
	buttons.push_back(*new Button(Button::DOWNBUTTON, { 400.f, 475.f }, { 100.f, 62.5f }, "MOVEDOWN"));

	buttons.push_back(*new Button(Button::RUNBUTTON, { 0.f, 537.5f }, { 500.f, 62.5f }, "RUN"));

	
}
void ScriptUI::run()
{
	window.create(sf::VideoMode(500, 600), "Scripting Engine Test Pre-Alpha", sf::Style::Close | sf::Style::Titlebar);

	sf::Event sEvent;
	auto running = true;

	// ReSharper disable once CppLocalVariableMayBeConst
	auto selected = false;
	//int selection;

	// ReSharper disable once CppJoinDeclarationAndAssignment
	Command::CommandType comAct;

	auto mouseHeld = false;
	sf::Vector2f mouseClick;
	auto prevClick = 0.f;

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
				if (mouseHeld)
				{
					if (sEvent.mouseButton.y == -858993460)
						commandList.handleScroll(float(sEvent.mouseMove.y), { mouseClick.x, prevClick });
					prevClick = float(sEvent.mouseMove.y);
				}

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
						// ReSharper disable once CppJoinDeclarationAndAssignment
						comAct = add.run();
						if (comAct != Command::NOCOMMAND)
						{
							addCommand(edit.run(int(commands.size()), comAct));
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
					mouseHeld = true;
					mouseClick = { float(sEvent.mouseButton.x), float(sEvent.mouseButton.y) };
					break;
				case sf::Event::MouseButtonReleased:
					mouseHeld = false;
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

		commandList.scrollbar.updateSize(int(commands.size()));

		window.clear(sf::Color::Blue);
		window.draw(commandList);
		for (auto l = 0; l < int(buttons.size()); l++)
		{
			window.draw(buttons[l]);
		}
		window.display();
	}
}
void ScriptUI::addCommand(Command com)
{
	commands.push_back(com);
	commandList.addCommand(com);
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
	buttons.push_back(*new Button(Button::ADDPORTRAIT, {0.f, 0.f}, {125.f, 60.f}, "ADDPORTRAIT"));
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


// OPTION
ScriptUI::Edit::Option::Option(): type(), selected(0) {}

// RADIO
ScriptUI::Edit::RadioOption::Radio::Radio(sf::Vector2f pos, float size, std::string text) : selected(false)
{
	this->pos = pos;
	this->size = size;
	this->str = text;

	rect.left = pos.x;
	rect.top = pos.y;
	rect.width = size;
	rect.height = size;
	shape.setPosition(pos);
	shape.setRadius(size / 2.f);
	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(1.f);
	this->text.setFont(font);
	this->text.setCharacterSize(30);
	this->text.setFillColor(sf::Color::Black);
	this->text.setPosition(pos.x + ceil(size * 1.5f), pos.y - 15.f);
	this->text.setString(text);
}
void ScriptUI::Edit::RadioOption::Radio::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(text, states);
}

// RADIOOPTION
ScriptUI::Edit::RadioOption::RadioOption()
{
	type = RADIOOPTION;
	selected = 0;
}
ScriptUI::Edit::RadioOption::RadioOption(std::vector<Radio> radios)
{
	type = RADIOOPTION;
	selected = 0;

	this->radios = radios;
	this->radios[0].selected = true;
	this->radios[0].shape.setFillColor(sf::Color::Black);
}

void ScriptUI::Edit::RadioOption::init(std::vector<Radio> radios)
{
	selected = 0;
	this->radios = radios;
	this->radios[0].selected = true;
	this->radios[0].shape.setFillColor(sf::Color::Black);
}

std::string ScriptUI::Edit::RadioOption::handleClick(sf::Vector2f mouse)
{
	for (auto l = 0; l < int(radios.size()); l++)
	{
		if (radios[l].rect.contains(mouse))
		{
			for (auto lx = 0; lx < int(radios.size()); lx++)
			{
				radios[lx].selected = false;
				radios[lx].shape.setFillColor(sf::Color::White);
			}
			radios[l].selected = true;
			radios[l].shape.setFillColor(sf::Color::Black);
			selected = l;
		}
	}

	return "";
}

void ScriptUI::Edit::RadioOption::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto l = 0; l < int(radios.size()); l++)
	{
		target.draw(radios[l], states);
	}
}

// FILESELECT
ScriptUI::Edit::FileSelect::FileSelect() : selected(-1), fontHeight(0)
{
}

ScriptUI::Edit::FileSelect::FileSelect(sf::Vector2f pos, sf::Vector2f size, std::string path) :
	selected(-1),
	fontHeight(21),
	scrollbar({ pos.x + (size.x - 15.f), pos.y }, { 15.f, size.y }, fontHeight)
{
	type = FILESELECTOPTION;

	this->pos = pos;
	this->size = size;
	this->path = path;

	rect.left = pos.x;
	rect.top = pos.y;
	rect.width = size.x;
	rect.height = size.y;

	shape.setPosition(pos);
	shape.setSize(size);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);

	overB.setPosition({ pos.x + 1.f, pos.y + size.y + 1.f });
	overB.setSize(size);
	overB.setFillColor(sf::Color::White);
	overR.setPosition({ pos.x + size.x + 1.f, pos.y + 1.f });
	overR.setSize(size);
	overR.setFillColor(sf::Color::White);
	overT.setPosition({ pos.x, pos.y - 1.f });
	overT.setSize({ size.x, -size.y });
	overT.setFillColor(sf::Color::White);

	this->FileSelect::setDirectory(path, "portraits");
}
void ScriptUI::Edit::FileSelect::hover(sf::Vector2f mouse)
{
	scrollbar.scrollUp.hover(mouse);
	scrollbar.scrollDown.hover(mouse);
	scrollbar.thumb.hover(mouse);
}
std::string ScriptUI::Edit::FileSelect::handleClick(sf::Vector2f mouse)
{
	for (auto l = 0; l < int(files.size()); l++)
	{
		if (files[l].rect.contains(mouse))
		{
			for (auto lx = 0; lx < int(files.size()); lx++)
			{
				files[lx].selected = false;
			}
			files[l].selected = true;
			return files[l].filename;
		}
	}
	return "";
}
void ScriptUI::Edit::FileSelect::handleScroll(float mouse, sf::Vector2f initial)
{
	const auto prevThumb = scrollbar.thumbPos;

	if (scrollbar.thumb.rect.contains(initial))
	{
		scrollbar.updatePos(mouse - initial.y);
	}

	auto scrollRatio = 0.f;

	if (float(files.size()) * fontHeight - rect.height > 0)
	{
		scrollRatio = (float(files.size()) * fontHeight + 2.f - rect.height) / (scrollbar.rect.height - scrollbar.thumb.rect.height);
	}

	for (auto l = 0; l < int(files.size()); l++)
	{
		files[l].setPosition(files[l].text.getPosition().x,
			files[l].text.getPosition().y +
			scrollRatio *
			(prevThumb - scrollbar.thumbPos));
	}
}
void ScriptUI::Edit::FileSelect::addFile(std::string str)
{
	files.push_back(*new File(str, { pos.x + 6.f, pos.y + -9.f + files.size() * fontHeight }, fontHeight));
}
void ScriptUI::Edit::FileSelect::setDirectory(std::string dir, std::string dir2)
{
	files.clear();

	path = dir;

	std::ostringstream oss;
	//const auto pa = fs::current_path();
	oss << dir;

	for (auto &p : fs::directory_iterator(oss.str()))
	{
		std::ostringstream poss;
		poss << p;
		const int ps = poss.str().find(dir2) + dir2.size() + 1;
		auto px = poss.str().substr(ps);
		if (px.find(".") == std::string::npos)
		{
			px = px + "\\";
		}
		else
		{
			px = px.substr(0, px.find("."));
		}

		if (px.find("endarrow") != std::string::npos
			|| px.find("nextarrow") != std::string::npos
			|| px.find("portrait_frame_blue") != std::string::npos
			|| px.find("text_box_blue") != std::string::npos)
		{
			
		}
		else
		{
			this->addFile(px);
		}
	}

	scrollbar.resetPos();

	scrollbar.updateSize(files.size());
}
/*
void ScriptUI::Edit::FileSelect::setDirectory(fs::directory_iterator dir)
{
	std::ostringstream oss;
	const auto pa = fs::current_path();
	oss << pa << "\\portraits\\" << dir << "\\";
	std::string x = "portraits";
	for (auto &p : fs::directory_iterator(oss.str()))
	{
		std::ostringstream poss;
		poss << p;
		const int ps = poss.str().find(x) + x.size() + 1;
		auto px = poss.str().substr(ps);
		if (px.find(".") == std::string::npos)
		{
			px = px + "\\";
		}
		else
		{
			px = px.substr(0, px.find("."));
		}
		this->addFile(px);
	}
}
 */

void ScriptUI::Edit::FileSelect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	for(auto l = 0; l < int(files.size()); l++)
	{
		target.draw(files[l], states);
	}
	target.draw(scrollbar, states);
	target.draw(overB, states);
	target.draw(overR, states);
	target.draw(overT, states);
}

// FILESELECT SCROLLBAR
ScriptUI::Edit::FileSelect::ScrollBar::ScrollBar() :
	scrollUp(Button::SCROLLUPBUTTON, {0, 0}, {0, 0}, ""),
	scrollDown(Button::SCROLLDOWNBUTTON, {0, 0}, {0, 0}, ""),
	thumb(Button::SCROLLTHUMBBUTTON, {0, 0}, {0, 0}, ""), fontHeight(0)
{
}

ScriptUI::Edit::FileSelect::ScrollBar::ScrollBar(sf::Vector2f pos, sf::Vector2f size, int fontHeight) :
	scrollUp(Button::SCROLLUPBUTTON, { pos }, { size.x, size.x }, ""),
	scrollDown(Button::SCROLLDOWNBUTTON, { pos.x, pos.y + (size.y - size.x) }, { size.x, size.x }, ""),
	thumb(Button::SCROLLTHUMBBUTTON, { pos.x, pos.y + size.x }, { size.x, size.y - 2 * size.x }, ""),
	fontHeight(fontHeight)
{
	this->rect.left = pos.x;
	this->rect.top = pos.y;
	this->rect.width = size.x;
	this->rect.height = size.y - 2 * size.x;

	this->shape.setPosition(pos);
	this->shape.setSize({ size.x, size.y });
	this->shape.setFillColor(sf::Color::White);
	this->shape.setOutlineColor(sf::Color::Black);
	this->shape.setOutlineThickness(1.f);

	thumbPos = thumb.rect.top;

	scrollUp.pos = pos;
	scrollUp.size = size;
}
void ScriptUI::Edit::FileSelect::ScrollBar::updateSize(int lines)
{
	if (lines * fontHeight / rect.height >= 1.f)
		thumbSize = rect.height / (lines * fontHeight / rect.height);
	else
		thumbSize = rect.height;
	thumb.rect.height = thumbSize;
	thumb.shape.setSize({ rect.width, thumbSize });
}
void ScriptUI::Edit::FileSelect::ScrollBar::updatePos(float pos)
{
	if ((pos <= 0 || thumb.rect.top + thumb.rect.height <= scrollDown.rect.top)
		&& (pos >= 0 || thumb.rect.top >= scrollUp.rect.top + scrollUp.rect.height))
	{
		this->thumbPos += pos;

		thumb.pos.y += pos;
		thumb.rect.top += pos;
		thumb.shape.setPosition(thumb.shape.getPosition().x, thumb.shape.getPosition().y + pos);
	}
	if (thumb.rect.top + thumb.rect.height >= scrollDown.rect.top)
	{
		this->thumbPos = scrollDown.rect.top - thumb.rect.height;

		thumb.pos.y = scrollDown.rect.top - thumb.rect.height;
		thumb.rect.top = scrollDown.rect.top - thumb.rect.height;
		thumb.shape.setPosition(thumb.shape.getPosition().x, scrollDown.rect.top - thumb.rect.height);
	}
	if (thumb.rect.top <= scrollUp.rect.top + scrollUp.rect.height)
	{
		this->thumbPos = scrollUp.rect.top + scrollUp.rect.height;

		thumb.pos.y = scrollUp.rect.top + scrollUp.rect.height;
		thumb.rect.top = scrollUp.rect.top + scrollUp.rect.height;
		thumb.shape.setPosition(thumb.shape.getPosition().x, scrollUp.rect.top + scrollUp.rect.height);
	}
}
void ScriptUI::Edit::FileSelect::ScrollBar::resetPos()
{
	this->thumbPos = rect.top + rect.width;

	thumb.pos.y = rect.top + rect.width;
	thumb.rect.top = rect.top + rect.width;
	thumb.shape.setPosition(thumb.shape.getPosition().x, rect.top + rect.width);
}
void ScriptUI::Edit::FileSelect::ScrollBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape, states);
	target.draw(thumb, states);
	target.draw(scrollUp, states);
	target.draw(scrollDown, states);
}

// FILE
ScriptUI::Edit::FileSelect::File::File() : ID(-1), selected(false), fontHeight(0)
{
	text.setFont(font);
}
ScriptUI::Edit::FileSelect::File::File(std::string str, sf::Vector2f pos, int fontHeight) : ID(-1), selected(false), fontHeight(fontHeight)
{
	filename = str;

	text.setFont(font);
	text.setString(str);
	text.setFillColor(sf::Color::Black);
	text.setPosition(pos);

	rect.left = pos.x - 6.f + 1.f;
	rect.top = pos.y + 13.f + 1.f;
	rect.width = 105.f - 5.f;
	rect.height = 20.f;

	shape.setPosition({ pos.x - 6.f, pos.y + 13.f });
	shape.setSize({ 105.f, float(fontHeight) });
	shape.setFillColor(sf::Color::Blue);
}
void ScriptUI::Edit::FileSelect::File::setPosition(sf::Vector2f pos)
{
	text.setPosition(pos);
	shape.setPosition({ pos.x - 6.f, pos.y + 13.f });
	rect.left = pos.x - 6.f + 1.f;
	rect.top = pos.y + 13.f + 1.f;
}
void ScriptUI::Edit::FileSelect::File::setPosition(float x, float y)
{
	text.setPosition({x, y});
	shape.setPosition({ x - 6.f, y + 13.f });
	rect.left = x - 6.f + 1.f;
	rect.top = y + 13.f + 1.f;
}
void ScriptUI::Edit::FileSelect::File::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (selected)
	{
		target.draw(shape, states);
	}
	target.draw(text, states);
}

// EDIT
ScriptUI::Edit::Edit()
{
	buttons.push_back(*new Button(Button::EDITCANCEL, { 0.f, 225.f }, { 200.f, 75.5f }, "EDITCANCEL"));
	buttons.push_back(*new Button(Button::EDITACCEPT, { 200.f, 225.f }, { 200.f, 75.5f }, "EDITACCEPT"));
}
ScriptUI::Command ScriptUI::Edit::run(int ID, Command::CommandType type)
{
	sf::Event eEvent;

	auto running = true;

	std::ostringstream path;
	path << fs::current_path();

	std::vector<sf::Text> text;
	std::vector<Option*> options;

	std::string comStr = "";
	std::ostringstream oComStr;

	switch (type)
	{
	case Command::PORTRAIT:
		window.create(sf::VideoMode(400, 300), "Edit Command", sf::Style::Close | sf::Style::Titlebar);

		text.push_back(sf::Text("Position", font));
		text[0].setStyle(sf::Text::Underlined);
		text[0].setPosition(25.f, -7.f);
		text[0].setFillColor(sf::Color::Black);

		text.push_back(sf::Text("Direction", font));
		text[1].setStyle(sf::Text::Underlined);
		text[1].setPosition(25.f, 112.f);
		text[1].setFillColor(sf::Color::Black);

		text.push_back(sf::Text("File Select", font));
		text[2].setStyle(sf::Text::Underlined);
		text[2].setPosition(225.f, -7.f);
		text[2].setFillColor(sf::Color::Black);

		options.push_back(new RadioOption({
			*new RadioOption::Radio({ 15.f, 35.f }, 15.f, "Left"),
			*new RadioOption::Radio({ 15.f, 55.f }, 15.f, "Right"),
			*new RadioOption::Radio({ 15.f, 75.f }, 15.f, "Top Left"),
			*new RadioOption::Radio({ 15.f, 95.f }, 15.f, "Top Right")}));
		options.push_back(new RadioOption({
			*new RadioOption::Radio({ 15.f, 155.f }, 15.f, "Left"),
			*new RadioOption::Radio({ 15.f, 175.f }, 15.f, "Right")}));
		path << "\\portraits\\";
		options.push_back(new FileSelect({ 150.f, 35.f }, { 120.f, 180.f }, path.str()));
		options.push_back(new FileSelect({ 270.f, 35.f }, { 120.f, 180.f }, path.str()));

		break;
	case Command::DIALOGUE:

		break;

	default:
		return *new Command(0, Command::NOCOMMAND, "");
	}

	auto accept = false;

	auto mouseHeld = false;
	sf::Vector2f mouseClick;
	auto prevClick = 0.f;

	std::string tempStr[2] = { "", "" };

	while (running)
	{
		while (window.pollEvent(eEvent))
		{
			std::string dir = "";
			std::ostringstream oss;

			if (mouseHeld)
			{
				for (auto l = 0; l < int (options.size()); l++)
				{
					if (eEvent.mouseButton.y == -858993460)
						options[l]->handleScroll(float(eEvent.mouseMove.y), { mouseClick.x, prevClick });
				}
				prevClick = float(eEvent.mouseMove.y);
			}

			switch (eEvent.type)
			{
			case sf::Event::MouseButtonPressed:
				mouseHeld = true;
				mouseClick = { float(eEvent.mouseButton.x), float(eEvent.mouseButton.y) };
				for (auto l = 0; l < int(options.size()); l++)
				{
					dir = options[l]->handleClick({ float(eEvent.mouseButton.x), float(eEvent.mouseButton.y) });
					if (dir != "")
					{
						if (l < int(options.size() - 1))
						{
							oss << path.str() << dir << "\\";
							options[l + 1]->setDirectory(oss.str(), dir);

							tempStr[0] = dir;
						}
						else
						{
							tempStr[1] = dir;
						}
					}
				}
				for (auto l = 0; l < int(buttons.size()); l++)
				{
					switch (handleClick({ eEvent.mouseButton.x, eEvent.mouseButton.y }))
					{
					case Button::EDITCANCEL:
						running = false;
						break;
					case Button::EDITACCEPT:
						accept = true;
						running = false;
						break;

					default:
						break;
					}
				}
				break;
			case sf::Event::MouseButtonReleased:
				mouseHeld = false;
				break;


			case sf::Event::Closed:
				running = false;
				break;
			default:
				break;
			}
		}

		for (auto l = 0; l < int(buttons.size()); l++)
		{
			buttons[l].hover(sf::Vector2f(sf::Mouse::getPosition(window)));
		}
		for (auto l = 0; l < int(options.size()); l++)
		{
			if (options[l]->type == Option::FILESELECTOPTION)
			{
				options[l]->hover(sf::Vector2f(sf::Mouse::getPosition(window)));
			}
		}



		window.clear(sf::Color::White);
		for (auto l = 0; l < int(options.size()); l++)
		{
			window.draw(*options[l]);
		}
		for (auto l = 0; l < int(buttons.size()); l++)
		{
			window.draw(buttons[l]);
		}
		for (auto l = 0; l < int(text.size()); l++)
		{
			window.draw(text[l]);
		}
		window.display();
	}

	switch (type)
	{
	case Command::PORTRAIT:
		oComStr << "@" << options[0]->selected << " " << tempStr[0] << tempStr[1] << " ";
		switch (options[1]->selected)
		{
		case 0:
			oComStr << "left";
			break;
		case 1:
			oComStr << "right";
			break;
		case 2:
			oComStr << "topleft";
			break;
		case 3:
			oComStr << "topright";
			break;
		default:
			break;
		}
		break;
	case Command::DIALOGUE:
		break;
	default:;
	}
	

	window.close();
	if (accept)
	{
		return *new Command(ID, type, oComStr.str());
	}
	return *new Command(-1, Command::NOCOMMAND, "");
}
// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppParameterNeverUsed
// ReSharper disable once CppMemberFunctionMayBeConst
ScriptUI::Command ScriptUI::Edit::run(int ID, Command com)
{
	switch (com.type)
	{
	case Command::PORTRAIT:
		
		break;
	case Command::DIALOGUE:
		
		break;

	default:
		return *new Command(0, Command::NOCOMMAND, "");
	}

	return *new Command(0, Command::NOCOMMAND, "");
}

ScriptUI::Button::ButtonType ScriptUI::Edit::handleClick(sf::Vector2i pos)
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
