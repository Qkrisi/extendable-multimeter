#ifndef TSGRAPHICS
#define TSGRAPHICS

#include <ArxContainer.h>
#include "URTouch.h"
#include "Adafruit_GFX.h"     
#include "Adafruit_ILI9341.h"
#include "MultimeterClasses.h"

using namespace arx;

namespace Multimeter
{
	enum Movement {Up, Down, Left, Right};

	class Measurement;
	
	inline int getY(int base);
	
	class Move
	{
		public:
			inline Move(Movement moveType, int value);
			inline Move(Movement moveType);
			Move(){}
			Movement MoveType;
			int Value;
	};
	
	class Button
	{
		public:
			inline Button(int x1, int y1, int x2, int y2, Measurement *handler);
			Button(){}
			inline bool Check(int x, int y);
			Measurement* Handler;
		private:
			int X1;
			int X2;
			int Y1;
			int Y2;
	};
	
	class Symbol
	{
		public:
			inline Symbol(int X, int Y, vector<Move> Movements);
			inline Button Draw(int X, int Y, Adafruit_ILI9341 *tft, Measurement *handler);
		private:
			int x;
			int y;
			vector<Move> movements;
	};
}

#endif
