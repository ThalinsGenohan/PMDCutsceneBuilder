//ScriptUI.h - Declares the script-building UI class

#pragma once

#include <filesystem>

#include <SFML/Graphics.hpp>

namespace fs = std::experimental::filesystem;

class ScriptUI
{
	sf::Font font;

	struct Button : sf::Drawable
	{
		enum ButtonType
		{
			NOBUTTON,					// Null value
			NEWBUTTON,					// New file
			OPENBUTTON,					// Open file
			SAVEBUTTON,					// Save file
			SAVEASBUTTON,				// Save file as
			CONFIGBUTTON,				// Configure program/file

			ADDBUTTON,					// Add script action
			REMOVEBUTTON,				// Remove selected action
			EDITBUTTON,					// Edit selected action
			UPBUTTON,					// Move selected action up
			DOWNBUTTON,					// Move selected action down

			RUNBUTTON,					// Run the current script

			SCROLLUPBUTTON,				// Scroll the textbox up
			SCROLLDOWNBUTTON,			// Scroll the textbox down
			SCROLLTHUMBBUTTON,			// Thumb for the scrollbar


			ADDPORTRAIT,				// Add portrait action
			ADDDIALOGUE,				// Add dialogue action


			EDITCANCEL,					// Edit cancel
			EDITACCEPT					// Edit accept
		};
		ButtonType type;						// What the button does when pressed

		sf::RectangleShape shape;				// Visual rectangle
		sf::Rect<float> rect;					// Mouse-detection rectangle
		sf::Texture texture;					// Texture for shape

		sf::Vector2f pos;						// Position of the button
		sf::Vector2f size;						// Size of the button

		/**
		 *	@brief Constructor
		 *	@param type What the button will do
		 *	@param pos Position of the button
		 *	@param size Size of the button
		 *	@param filename The image to be loaded for the button
		 */
		Button(ButtonType type, sf::Vector2f pos, sf::Vector2f size, std::string filename = "");
		
		/** 
		 *	@brief Checks if the mouse is hovering over a button and turns the button red if true.
		 *	@param m Position of the mouse
		 */
		bool hover(sf::Vector2f m);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
	std::vector<Button> buttons;		// All buttons on the main window

	struct Command
	{
		enum CommandType
		{
			NOCOMMAND,		// Null value
			PORTRAIT,		// Portrait action
			DIALOGUE		// Dialogue action
			//...
		};
		CommandType type;			// What the command will do

		int ID;						// ID of the command (position on the list)

		sf::Rect<float> rect;		// Rect for detecting mouse clicks for selection
		sf::Text text;				// Text to be displayed in the main window for this command

		/**
		 *	@brief Constructor
		 *	@param ID ID of the command (position on the list)
		 *	@param type What the command will do
		 */
		explicit Command(int ID, CommandType type);

		/**
		 *	@brief Pulls up the editing window for the command
		 */
		void edit();
	};
	std::vector<Command> commands;		// All the commands written


	struct CommandList : sf::Drawable
	{
		int selected;					// ID of the selection (-1 if none)

		sf::Rect<float> rect;			// Rect of the textbox
		sf::RectangleShape shape;		// Visible shape of the textbox

		int fontHeight = 21;

		struct ScrollBar : Drawable
		{
			sf::Rect<float> rect;		// Rect of the scrollbar
			sf::RectangleShape shape;	// Visible shape of the scrollbar

			Button scrollUp;			// Scroll up button
			Button scrollDown;			// Scroll down button
			Button thumb;				// Thumb button

			float thumbSize = 50.f;		// = barSize / ((lines * lineSize) / barSize)
			float thumbPos = 0.f;		// Thumb position relative to the scrollbar
			int fontHeight;

			ScrollBar();
			ScrollBar(sf::Vector2f pos, sf::Vector2f size, int fontHeight);

			/**
			*	@brief Updates the size of the thumb
			*	@param lines Number of lines in the textbox
			*/
			void updateSize(int lines);
			/**
			*	@brief Updates position of the thumb
			*/
			void updatePos(float pos);

			void resetPos();

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		};
		ScrollBar scrollbar;			// The textbox's scrollbar

		struct CommandText : Drawable
		{
			sf::Rect<float> rect;		// Rect of the command text
			sf::RectangleShape shape;	// Shape of the command
			sf::Text text;				// Text describing the command

			CommandText();

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		};
		std::vector<CommandText> options;

		CommandList();

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
	CommandList commandList;			// The command list, conatining the commands in text

	sf::RenderWindow window;			// The main Scripting UI window

	class Add
	{
		sf::RenderWindow window;		// The "Add Command" subwindow
		std::vector<Button> buttons;	// All the buttons for the subwindow

	public:
		Add();

		// Main loop for the subwindow
		Command::CommandType run();
		// Handles mouse clicks in the subwindow
		Button::ButtonType handleClick(sf::Vector2i pos);
	};
	Add add;							// The "Add Command" subwindow
	class Edit
	{
		sf::Font font;

		sf::RenderWindow window;		// The "Edit Command" subwindow
		std::vector<Button> buttons;	// All the buttons for the subwindow

		struct Option : sf::Drawable
		{
			enum OptionType
			{
				NOOPTION,
				RADIOOPTION,
				FILESELECTOPTION
			};
			OptionType type;

			virtual void hover(sf::Vector2f mouse) {}

			// ReSharper disable once CppParameterNeverUsed
			virtual std::string handleClick(sf::Vector2f mouse) { return ""; }

			virtual void handleScroll(float mouse, sf::Vector2f initial) {}

			virtual void setDirectory(std::string dir, std::string dir2) {}

		private:
			// ReSharper disable once CppOverridingFunctionWithoutOverrideSpecifier
			// ReSharper disable CppParameterNeverUsed
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}
			// ReSharper restore CppParameterNeverUsed
		};
		struct RadioOption : Option
		{
			struct Radio : Drawable
			{

				sf::Rect<float> rect;
				sf::CircleShape shape;
				sf::Font font;
				sf::Text text;

				sf::Vector2f pos;
				float size;

				bool selected;

				Radio(sf::Vector2f pos, float size, std::string text);

			private:
				void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
			};
			std::vector<Radio> radios;

			int selected;

			RadioOption();
			explicit RadioOption(std::vector<Radio> radios);

			void init(std::vector<Radio> radios);

			std::string handleClick(sf::Vector2f mouse) override;

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		};
		struct FileSelect : Option
		{
			sf::Vector2f pos;
			sf::Vector2f size;
			std::string path;

			int selected;

			sf::Rect<float> rect;
			sf::RectangleShape shape;
			sf::RectangleShape overB;
			sf::RectangleShape overR;
			sf::RectangleShape overT;

			int fontHeight;

			struct ScrollBar : Drawable
			{
				sf::Rect<float> rect;		// Rect of the scrollbar
				sf::RectangleShape shape;	// Visible shape of the scrollbar

				Button scrollUp;			// Scroll up button
				Button scrollDown;			// Scroll down button
				Button thumb;				// Thumb button

				float thumbSize = 50.f;		// = barSize / ((lines * lineSize) / barSize)
				float thumbPos = 0.f;		// Thumb position relative to the scrollbar
				int fontHeight;

				ScrollBar();
				ScrollBar(sf::Vector2f pos, sf::Vector2f size, int fontHeight);

				/**
				*	@brief Updates the size of the thumb
				*	@param lines Number of lines in the textbox
				*/
				void updateSize(int lines);
				/**
				*	@brief Updates position of the thumb
				*/
				void updatePos(float pos);

				void resetPos();

			private:
				void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
			};
			ScrollBar scrollbar;

			struct File : Drawable
			{
				int ID;
				std::string filename;
				
				bool selected;

				sf::Rect<float> rect;
				sf::RectangleShape shape;
				sf::Font font;
				sf::Text text;

				int fontHeight;

				File();
				explicit File(std::string str, sf::Vector2f pos, int fontHeight);
				
				void setPosition(sf::Vector2f pos);
				void setPosition(float x, float y);

			private:
				void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
			};
			std::vector<File> files;

			FileSelect();
			FileSelect(sf::Vector2f pos, sf::Vector2f size, std::string path);

			void hover(sf::Vector2f mouse) override;

			std::string handleClick(sf::Vector2f mouse) override;
			void handleScroll(float mouse, sf::Vector2f initial) override;

			void addFile(std::string str);
			void setDirectory(std::string dir, std::string dir2) override;
			//void setDirectory(fs::directory_iterator dir);

		private:
			void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		};

		std::vector<Option> options;

	public:
		Edit();

		/**
		 *	@brief The main loop of the Edit subwindow.
		 *	@n To be used for new, empty commands.
		 *	@param ID ID of the command to be edited
		 *	@param type Type of command to be edited
		 */
		Command run(int ID, Command::CommandType type);
		/**
		*	@brief The main loop of the Edit subwindow.
		*	@n To be used for existing commands.
		*	@param ID ID of the command to be edited
		*	@param com The command to be edited
		*/
		Command run(int ID, Command com);

		// Handles mouse clicks in the window
		Button::ButtonType handleClick(sf::Vector2i pos);
	};
	Edit edit;							// The "Edit Command" subwindow
	class File
	{
		
	public:
		File();
	};
	//File file;						// The (unimplemented) "Open/Save File" subwindow
	class Config
	{
		
	public:
		Config();
	};
	//Config config;					// The (unimplemented) "Configuration" subwindow

public:

	ScriptUI();

	// The main loop of the main window.
	void run();

	// Handles mouse clicks in the window
	Button::ButtonType handleClick(sf::Vector2i pos);

};