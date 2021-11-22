#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
struct COORD {int x;int y;};

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
 3. In your main code, initialize the class somewhere, in the following way:
 
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
	>>  canvas.append(foxPicture, {0,25}); //'location'={0,25}, to place the drawing leftmost, and toward the bottom of the whole tapestry.

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

	>>  class TetrisGame : class Grid { public:
	>>		void prepare()
	>>			{****Write your implementation of this method here...****}

What 'prepare()' is supposed to do, is look at the information stored in the 'data' attribute,
 and change either its own drawing, or a drawing of one of its subGrids, in a desired way.
The same format as above applies to another function, 'addData()', by which the object would
 take in new data in a desired way.

	>>		void addData(T data)
	>>			{****Write your implementation of this method here....****}
	>>  };


*/
template <typename T>
class Grid
{
public:
	//////////////////////////////////////
	// Attributes

	COORD location;
	std::string drawing;
	COORD size; //Size of the drawing.

	Grid* parent;
	std::vector<Grid> subGrids;

	T data; //Most recent

	/////////////////////////////////////
	// Constructors
	Grid(COORD size) //(To make the beginning canvas.)
	{
		this->setSize(size);
	}
	Grid(std::string drawing) //(To make more additional grids)
	{
		this->setDrawing(drawing);
		this->setSize();
	}

	void append(Grid* subGrid, COORD location) //Attach a subgrid onto this current grid.
	{
		subGrid->setLocation(location);
		this->subGrids.append(subGrid);
	}

	//////////////////////////////////
	// Setters:
	void setSize() { this->size = findSize(); }
	void setSize(COORD size) { this->size = size; }
	void setLocation(COORD location) { this->location = location; }
	void setDrawing(std::string drawing) { this->drawing = drawing; }
	void setParent(Grid* parent) { this->parent = parent; }
	void setSubGrids(std::vector<Grid> subGrids) { this->subGrids = subGrids; }
	void setData(T data) { this->data = data; }

	/////////////////////////////////
	// Key functions
	void display()
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), this->location);
		linesDown = 0;
		for (int i = 0; i < this->drawing.length(); i += 1)
		{
			if (drawing[i] != '\t') //Should treat as nothingness -- move cursor over one.	(So, if there was prior drawing underneath, then leave that character intact.)
			{
				std::cout << '%';//TODO: Actually implement this... probably using GetConsoleCursorPosition and SetConsoleCursorPosition.
			}
			if (drawing[i] == '\n')
			{
				linesDown += 1;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { this->location.x, this->location.y + linesDown });
			}
			else
			{
				std::cout << drawing[i - linesDown];
			}
		}
		//Then, draw each subGrid:
		for (std::vector<Grid>::iterator subGrid = this->subGrids.begin(); subGrid != this->subGrids.end(); ++subGrid)
		{
			subGrid.display();
		}
	}

	virtual void prepare()
	{
		//Prepare the drawing, based on the most recent data that the object holds.
		//	(...Do nothing here, until a class that inherits from 'Grid' does something here.)
	}
	void prepare(T data)
	{
		this->addData(data);
		this->prepare();
	}

	virtual void addData(T data)
	{
		//Currently, this method simply replaces the currently data with the given data. 
		// However, it's left open for a future programmer, if they want a 'Grid' extension to take in the new information in a different way.
		// (For example, adding it to a list of recent data pieces...)
		this->setData(data);
	}

private:

	void setChar(COORD localLocation, char newChar) // Change a character in the 'drawing' string:
	{
		this->drawing[(localLocation.x + 1) * (localLocation.y + 1) - 1 + localLocation.y] = newChar; //TODO: Look over & make sure this logic is right.
		//	As goofy as 'localLocation' seems, it's to make it clear that this 'location' is relative to the Grid it's attached to, not necessarily the original canvas Grid object.
	}

	int findSize() //Find the 'size' dimensions based on the drawing.
	{
		int sizeX = 0;
		int maxSizeX = 0; 
		int sizeY = 0;
		for (int i = 0; i < this->drawing.length(); i += 1)
		{
			if (this->drawing[i] == '\n')
			{
				if (sizeX > maxSizeX)
				{
					maxSizeX = sizeX;
				}
				sizeX = 0;
				sizeY += 1;
			}
			else
			{
				sizeX += 1;
			}
		}
		if (sizeX > maxSizeX)
		{
			maxSizeX = sizeX;
		}
		return { maxSizeX, sizeY };
	}
};
