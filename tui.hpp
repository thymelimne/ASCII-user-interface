#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
/*
TUI: Text User Interface. Like a GUI, but it's text that gets printed to the console, then eventually printed over when the program updates it.
This TUI takes an object-oriented approach, with a tree structure of 'Grid' objects.
__________________
|                | <-- Here is an example of a picture you might draw.
|       ##       |	
|     ######     |	But, what if you wanted a dashboard about this plant, at a corner of the screen?
|   ##########   |		...Maybe, to display about how hydrated it is?
|     ######     |				
|       ##   #   |		That is why this program takes an object-oriented approach.
|       ## ##    |
|       ####     |			Each object holds something you want to draw, and where you want to draw it,
|       ##       |			so you could easily define another 'Grid' object, that you'd name 'dashboard',
%%%%%%%%%%%%%%%%%%			and then set its location to wherever you like on the canvas. (...& then reposition it.)








To use:
 1. First, note that the cruicial code for this relies on the SetConsoleCursorPosition() method given by <Windows.h>.
	So, it'll probably only work for your coding project if you're using Windows... Apologies, otherwise.
 2. Include this file as a header.
 3. In your main code, initialize this class somewhere, in the following way:
 
	>>  COORD size = {100,30}; //100 characters horizontally(x-axis), 30 characters vertically(y-axis).
	>>  Grid canvas{size}; //Make an empty canvas.

...Once you've set the basic canvas, then you can make new, modular 'Grid' objects like so...
(...Think of it as making smaller pictures to pin on top of your corkboard (the canvas)....)

 _._     _,-'""`-._
(,-.`._,'(       |\`-/|
    `-.-' \ )-`( , o o)
          `-    \`_`"'-

	>>  std::string drawing = " _._     _,-'""`-._\n(,-.`._,'(       |\`-/|\n    `-.-' \ )-`( , o o)\n          `-    \`_`"'-";
	>>  Grid foxPicture{drawing};
	>>  canvas.append(&foxPicture, {0,25}); //'location'={0,25}, to place the drawing leftmost, and toward the bottom of the whole tapestry.

			NOTES ABOUT LOCATION:
			 - The 'location' is where the top left corner of this picture should get placed
				(...Think of it as the 'thumbtack' of your picture, and you're placing it onto the board somewhere.)
			 - The location is relative to the parent Grid object -- not necessarily the initial canvas.
				(i.e., if you're placing your foxPicture onto another picture, let's say, 'couchPicture', then the location is in relation to where that couchPicture is.)
				
				canvas (size={100,20})
					coffeeTablePicture
						vasePicture
							flowersPicture
						magazinePicture
					prettyAtlasPicture
					couchPicture (location={25,10}; size={50,10}) //The couch is in the middle of the room, toward the bottom of the frame.
						foxPicture (location={0,2}; size={23,4}) //The top-left corner of the fox is very close to the top-left corner of the couch. This means that the fox, relative to the top left corner of the whole canvas, is 25+0=25 to the right and 10+2=12 down.

    ----+x---->
   |
  +y
   |
   V

To display your drawings, call the function:

	>>  canvas.display(); //Display all the drawings on the output console.

There is another class function, currently empty, called prepare(). This is left open for a programmer.
To use this function, create a class that inherits from Grid, and define prepare() in that.

	>>  class TetrisGame : public Grid { public:
	>>		void prepare()
	>>			{****Write your implementation of this method here...****}

What 'prepare()' is supposed to do, is look at the information stored in the 'data' attribute,
 and change either its own drawing, or a drawing of one of its subGrids, in a desired way.
The same format as above applies to another function, 'addData()', by which the object would
 take in new data in a desired way.

	>>		void addData(T data)
	>>			{****Write your implementation of this method here....****}
	>>  };
















FUNCTIONS LIST:
public:
	Constructors
	Setters & getters
	append()
	display()
virtual public:
	prepare()
	addData()
	initialDrawing()
protected:
	setChar()
	findSize()


ATTRIBUTES LIST:
location
drawing
size
parent
subGrids
data








*/
class Grid
{
public:

	/*.- -.-. -.-. --- .-. -.. .. -. --. / - --- / .- .-.. .-.. / -.- -. --- .-- -. / .-.. .- .-- ... / --- ..-. / .- ...- .. .- - .. --*/
	//ATTRIBUTES
	COORD location;
	std::vector<std::string> drawing;
	COORD size;
	Grid* parent;
	std::vector<Grid*> subGrids;
	std::string data;


	/*- -. --..-- / - .... . .-. . / .. ... / -. --- / .-- .- -.-- / .- / -... . . / ... .... --- ..- .-.. -.. / -... . / .- -... .-.. . */
	//CONSTRUCTORS
	Grid(COORD size) 
	{ 
		this->setSize(size); 
	}
	Grid(std::string drawing)
	{
		this->generateDrawingFromString(drawing);
		this->setSize();
	}
	Grid()
	{
		this->initialDrawing();
		this->setSize();
	}
	void append(Grid* subGrid, COORD location)
	{
		(*subGrid).location = location;
		(*subGrid).parent = this;
		this->subGrids.push_back(subGrid);
	}


	/* - --- / ..-. .-.. -.-- .-.-.- / .. - ... / .-- .. -. --. ... / .- .-. . / - --- --- / ... -- .- .-.. .-.. / - --- / --. . - / .. */
	// SETTERS, GETTERS:
	void setSize() { this->size = this->findSize(); }//Deliberately overloaded method
	void setSize(COORD size) { this->size = size; }
	void setLocation(COORD location) { this->location = location; }
	void setDrawing(std::vector<std::string> drawing) { this->drawing = drawing; }
	void setDrawing(std::string drawing) 
	{ 
		std::cout << "setDrawing()" << '\n';
		this->drawing = this->generateDrawingFromString(drawing); 
		std::cout << "Done with generateDrawingFromString()" << '\n';
	} //(Another overloaded setter here...)
	void setParent(Grid* parent) { this->parent = parent; }
	void setSubGrids(std::vector<Grid*> subGrids) { this->subGrids = subGrids; }
	void setData(std::string data) { this->data = data; }
	void setChar(COORD loc, char character) { this->drawing[loc.Y][loc.X] = character; }
	/*- ... / ..-. .- - / .-.. .. - - .-.. . / -... --- -.. -.-- / --- ..-. ..-. / - .... . / --. .-. --- ..- -. -.. .-.-.- / - .... . / */
	COORD getSize() { return this->size; }
	COORD getLocation() { return this->location; }
	std::vector<std::string> getDrawing() { return this->drawing; }
	Grid* getParent() { return this->parent; }
	std::vector<Grid*> getSubGrids() { return this->subGrids; }
	std::string getData() { return this->data; }


	/*-... . . --..-- / --- ..-. / -.-. --- ..- .-. ... . --..-- / ..-. .-.. .. . ... / .- -. -.-- .-- .- -.-- / -... . -.-. .- ..- ... */
	//KEY FUNCTIONS
	void display()
	{
		for (size_t i = 0; i < this->drawing.size(); i++) 
		{ /*First, display the drawing of this Grid:*/
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { this->location.X, (short)(this->location.Y + i) });
			std::cout << this->drawing[i];
		}
		for (std::vector<Grid*>::iterator it = this->subGrids.begin(); it != this->subGrids.end(); ++it) 
		{ /*Then, display each subGrid:*/
			(*it[0]).display();
		}
	}

	virtual void prepare() {}//Prepare the drawing, based on the most recent data that the object holds.  (...Do nothing here, until a class that inherits from 'Grid' does something here.)
	void prepare(std::string data)
	{
		this->addData(data);
		this->prepare();
	}

	virtual void addData(std::string data) { this->setData(data); } //Currently, this method simply replaces the currently data with the given data. However, it's left open for a future programmer, if they want a 'Grid' extension to take in the new information in a different way. (For example, adding it to a list of recent data pieces...)
	
	virtual void initialDrawing() {}//Left 'virtual' for when it gets re-implemented in an extension of this class.
	
	

/*. / -... . . ... / -.. --- -. .----. - / -.-. .- .-. . / .-- .... .- - / .... ..- -- .- -. ... / - .... .. -. -.- / .. ... / .. --*/
protected:

	//An option given to print a line on the display... normally intended to be used by extensions of this class:
	void printFromLoc(std::string toPrint, COORD loc)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), loc);
		std::cout << toPrint;
	}
	void addToDrawing(COORD localLocation, std::string string)
	{
		short xindex = 0;
		short yindex = 0;
		for (size_t i = 0; i < string.size(); i++)
		{
			if (string[i] == '\n')
			{
				xindex = 0;
				yindex += 1;
			}
			else
			{
				setChar({ (short)(localLocation.X + xindex + i), (short)(localLocation.Y + yindex) }, string[i]);
			}
		}
	}
	COORD findSize() //Find the 'size' dimensions based on the drawing.
	{
		size_t sizeY = this->drawing.size();
		size_t sizeX = 0;
		for (size_t i = 0; i < this->drawing.size(); i++)
		{
			if (this->drawing[i].length() > sizeX)
			{
				sizeX = this->drawing[i].length();
			}
		}
		return { (short)sizeX, (short)sizeY };
	}
	std::vector<std::string> generateDrawingFromString(std::string drawing)
	{
		std::cout << drawing;
		std::vector<std::string> stringToReturn;
		std::string currentLine = "";
		for (size_t i = 0; i < drawing.size(); i++)
		{
			if (drawing[i] != '\n')
			{
				std::cout << drawing[i] << '\n';
				currentLine += drawing[i];
				
			}
			else
			{
				stringToReturn.push_back(currentLine);
				currentLine = "";
			}
		}
		return stringToReturn;
	}
};

/*.--. --- ... ... .. -... .-.. . .-.-.- */
