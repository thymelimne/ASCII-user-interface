#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
struct COORD {int x;int y;};

/*
TUI: Text User Interface. Like a GUI, but it's text that gets printed to the console, then eventually printed over when the program updates it.
This TUI takes an object-oriented approach, with a tree structure of 'Grid' objects.
_________________
                |		<--- Here is an example of a picture you might draw.
       ##       |	
     ######     |			But, what if you wanted a dashboard about this plant, at a corner of the screen?
   ##########   |												...Maybe, to display about how hydrated it is?
     ######     |				
       ##   #   |				That is why this program takes an object-oriented approach.
       ## ##    |
       ####     |					Each object holds something you want to draw, and where you want to draw it,
       ##       |					so you could easily define another 'Grid' object, that you'd name 'dashboard',
%%%%%%%%%%%%%%%%%					and then set its location to wherever you like on the canvas. (...& then reposition it.)








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

	>>  std::string drawing = " _._     _,-'""`-._\n(,-.`._,'(       |\`-/|\n    `-.-' \ )-`( , o o)\n          `-    \`_`"'-"
	>>  Grid foxPicture{drawing};
	>>  canvas.append(foxPicture, {0,25}) //'location'={0,25}, to place the drawing leftmost, and toward the bottom of the whole tapestry.

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

	>>  class tetrisGame : class Grid { public:
	>>		void prepare(T data)
	>>			{****Write your implementation of this method here...****}
	>>  };

What 'prepare()' is supposed to do, is take data that suggests how it should change its drawing, 
 and change either its own drawing, or a drawing of one of its subGrids, in a desired way.


*/
template <typename T>
class Grid
{
public:
	//////////////////////////////////////
	// Attributes

	COORD pos;
	std::string drawing;
	COORD size; //Size of the drawing.

	Grid* parent;
	std::vector<Grid> subGrids;

	T data; //Most recent

	/////////////////////////////////////
	// Constructors
	Grid(COORD size) //(To make the beginning canvas.)
	{
		this->size = size;
	}
	Grid(COORD size, std::string drawing, Grid* parent, COORD pos) //(To make a new drawing, later on in the process.)
	{
		setSize(size);
		setDrawing(drawing);
		setParent(parent);
		setPos(pos);
	}

	//Setters:
	void setSize(COORD size) { this->size = size; }
	void setPos(COORD pos) { this->pos = pos; }
	void setDrawing(std::string drawing) { this->drawing = drawing; }
	void setParent(Grid* parent) { this->parent = parent; }
	void setSubGrids(std::vector<Grid> subGrids) { this->subGrids = subGrids; }

	/////////////////////////////////
	// Key functions

	virtual void prepare()
	{
		//Prepare the drawing, based on the most recent data that the object holds.
		//	(...Do nothing here, until a class that inherits from 'Grid' does something here.)
	}
	void prepare(T data)
	{
		this->data = data;
		this->prepare();
	}
	void draw()
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), this->pos);


		linesDown = 0;
		for (int i = 0; i < this->drawing.length(); i += 1)
		{
			if (drawing[i] != '`') //Should treat as nothingness -- move cursor over one.
			{
				std::cout << '`';//TODO: Actually implement this... probably using GetConsoleCursorPosition and SetConsoleCursorPosition.
			}
			if (drawing[i] == '\n')
			{
				linesDown += 1;
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { this->pos.x, this->pos.y + linesDown });
			}
			else
			{
				std::cout << drawing[i - linesDown];
			}
		}
	}


private:

	void setChar(COORD localPos, char newChar)//Change a character in the 'drawing' string:
	{
		this->drawing[(localPos.x + 1) * (localPos.y + 1) - 1 + localPos.y] = newChar; //TODO: Look over & make sure this logic is right.
	}
};
