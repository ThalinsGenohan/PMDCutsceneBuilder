#pragma once

#include <SFML/Graphics.hpp>

class ScriptUI
{
	struct Button : sf::Drawable
	{
		enum ButtonType
		{
			NOBUTTON,
			NEWBUTTON,
			OPENBUTTON,
			SAVEBUTTON,
			SAVEASBUTTON,
			CONFIGBUTTON,

			ADDBUTTON,
			REMOVEBUTTON,
			EDITBUTTON,
			UPBUTTON,
			DOWNBUTTON,

			RUNBUTTON,

			SCROLLUPBUTTON,
			SCROLLDOWNBUTTON,


			ADDPORTRAIT,
			ADDDIALOGUE
		};
		ButtonType type;						// What the button does when pressed

		sf::RectangleShape shape;				// Visual rectangle
		sf::Rect<float> rect;					// Mouse-detection rectangle
		sf::Texture texture;					// Texture for shape

		sf::Vector2f pos;
		sf::Vector2f size;


		Button(ButtonType type, sf::Vector2f pos, sf::Vector2f size, std::string filename = "");

		////////////////////////////////////
		/// \brief Handles button highlights
		///
		/// \param m Position of the mouse
		////////////////////////////////////
		bool hover(sf::Vector2f m);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
	std::vector<Button> buttons;

	struct Command
	{
		enum CommandType
		{
			NOCOMMAND,
			PORTRAIT,
			DIALOGUE
			//...
		};
		CommandType type;

		int ID;

		sf::Rect<float> rect;
		sf::Text text;


		explicit Command(int ID, CommandType type);

		//////////////////////////////////////////////////////
		/// \brief Pulls up the editing window for the command
		///
		//////////////////////////////////////////////////////
		void edit();
	};
	std::vector<Command> commands;

	struct Select : sf::Drawable
	{
		int selected;

		sf::Rect<float> rect;
		sf::RectangleShape shape;

		struct ScrollBar : Drawable
		{
			sf::Rect<float> rect;
			sf::RectangleShape shape;

			Button scrollUp;
			Button scrollDown;
			Button scroll;

			float barSize = 50.f;	// = boxSize / ((lines * lineSize) / boxSize)
			float barPos = 82.5f;

			ScrollBar();

			void updateSize(int lines);
			void updatePos();

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		};
		ScrollBar scrollbar;

		struct SelOption : sf::Drawable
		{
			sf::Rect<float> rect;
			sf::RectangleShape shape;
			sf::Text text;

			SelOption();

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		};
		std::vector<SelOption> options;

		Select();

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
	Select commandList;

	sf::RenderWindow window;

	class Add
	{
		sf::RenderWindow window;
		std::vector<Button> buttons;

	public:
		Add();

		Command::CommandType run();

		Button::ButtonType handleClick(sf::Vector2i pos);
	};
	Add add;
	class Edit
	{
		sf::RenderWindow window;
		std::vector<Button> buttons;


		
	public:
		Edit();

		Command run(int ID, Command::CommandType type);
		Command run(int ID, Command com);
	};
	Edit edit;
	class File
	{
		
	public:
		File();
	};
	//File file;
	class Config
	{
		
	public:
		Config();
	};
	//Config config;

public:

	ScriptUI();

	void run();

	Button::ButtonType handleClick(sf::Vector2i pos);

};

/*
class ScriptUI
{
struct Button
{
enum ButtonType
{
NOBUTTON,

NEW,
LOAD,
SAVE,
SAVEAS,

CONFIG,

ADD,
REMOVE,
EDIT,
MOVEUP,
MOVEDOWN,

RUN,

SCROLLUP,
SCROLLDOWN,


ADDPORTRAIT,
ADDDIALOGUE,
ADDWAIT,
ADDBGM,
ADDSE,
ADDANIMATION,
ADDCAMERA,
ADDTRANSITION,
ADDBACKGROUND,
ADDPICTURE
};
ButtonType type;

Button(ButtonType type, sf::Vector2f pos, sf::Vector2f size, std::string filename = "");

sf::RectangleShape shape;
sf::Rect<float> rect;
sf::Texture texture;
sf::Sprite sprite;
};
std::vector<Button> buttons;
std::vector<Button> addButtons;

class Editor;

std::vector<Button> editButtons;

public:
ScriptUI();

void createWindow();
void closeWindow();
void showWindow();
void hideWindow();

void addCommand();
void editCommand(int selected);
void editCommand(Button::ButtonType type);

void run();

private:
sf::RenderWindow window;
sf::RenderWindow addWindow;
sf::RenderWindow editWindow;

struct Textbox : sf::Drawable
{
	struct Command : Drawable
	{
		enum CommandType
		{
			NOTYPE,
			PORTRAIT,
			DIALOGUE
		};

		Command();
		explicit Command(CommandType type);
		void add(std::string text);
		void edit();

	private:
		CommandType type;
		std::vector<std::string> args;

		std::string display;

		int num;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
std::vector<Command> commands;

sf::RectangleShape shape;
struct ScrollBar : Drawable
{
	std::vector<sf::Rect<float>> rects;
	std::vector<sf::RectangleShape> shapes;

	float barSize = 50.f;	// = 412.5f / ((lines * 18.f) / 412.5f)
	float barPos = 82.5f;

	void updateSize(int lines);
	void updatePos();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
ScrollBar scrollbar;

void updateScroll();

void addCommand(Command::CommandType type);

private:
sf::View view;

void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
Textbox textbox;


Button::ButtonType handleClick(sf::Vector2i pos);

};
*/