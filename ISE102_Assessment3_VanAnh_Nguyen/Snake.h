#ifndef __SNAKE__
#define __SNAKE__
#include "Point2d.h"
#include "textpixels_enums.h"
#include <vector>
using textpixels::Point2d;
using textpixels::Direction;
using std::vector;

class Snake
{
public:

	// Position and speed use data types/enums are defined in textpixels_enums.h
	Point2d location{ 1,1 };
	int xDir = Direction::NONE;
	int yDir = Direction::NONE;
	short colour = 0x0;
	bool expired = false;
	bool poisoned = false;
	int length = 1;
	vector<Point2d> history;
	// How many frames pass before it's time for Snakey to move again? A high update 
	// rate for the game means we capture all input but if snakey moves every frame 
	// she's too fast. Snakey doesn't have to move every frame. She's chill.
	int moveInterval = 10;
};

#endif
