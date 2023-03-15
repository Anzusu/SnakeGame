#ifndef __FRUIT__
#define __FRUIT__
#include "Point2d.h"
#include "textpixels_enums.h"
using textpixels::Point2d;
using textpixels::Direction;

class Fruit
{
public:

	Point2d location{ 1,1 };
	short colour = 0x0;
	int pointsValue = 1;
};

#endif