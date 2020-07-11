#include "MultimeterClasses.h"
#include <ArxContainer.h>
#include "URTouch.h"
#include "Adafruit_GFX.h"     
#include "Adafruit_ILI9341.h"

using namespace Multimeter;
using namespace arx;

Move::Move(Movement moveType, int value)
{
	MoveType = moveType;
	Value = value;
}

Move::Move(Movement moveType)
{
	MoveType = moveType;
	Value = 1;
}

Symbol::Symbol(int X, int Y, vector<Move> Movements)
{
	x = X;
	y = Y;
	movements = Movements;
}

int Multimeter::getY(int base)
{
	if(base==-1){
		return -1;
	}
	if(base>120){
		return 120-(base-120);
	}
	if(base < 120){
		return 120+(120-base);
	}
	return 120;
}

Button Symbol::Draw(int X, int Y, Adafruit_ILI9341 *tft, Measurement *handler)
{
	tft->drawLine(X, Y, X, Y+20, ILI9341_WHITE);
	tft->drawLine(X, Y+20, X+20, Y+20, ILI9341_WHITE);
	tft->drawLine(X+20, Y+20, X+20, Y, ILI9341_WHITE);
	tft->drawLine(X, Y, X+20, Y, ILI9341_WHITE);
	int baseX = X+x;
	int baseY = Y+y;
	for(int i = 0;i<movements.size(); i++)
	{
		int newX = baseX;
		int newY = baseY;
		auto currentMove = movements[i];
		switch(currentMove.MoveType)
		{
			case Up:
				newY -= currentMove.Value;
				break;
			case Down:
				newY += currentMove.Value;
				break;
			case Left:
				newX -= currentMove.Value;
				break;
			case Right:
				newX += currentMove.Value;
				break;
		}
		tft->drawLine(baseX, baseY, newX, newY, ILI9341_WHITE);
		baseX = newX;
		baseY = newY;
	}
	return Button(X, Y, X+20, Y+20, handler);
}

Button::Button(int x1, int y1, int x2, int y2, Measurement *handler)
{
	X1 = x1;
	X2 = x2;
	Y1 = y1;
	Y2 = y2;
	Handler = handler;
}

bool Button::Check(int x, int y)
{
	return x >= X1 && x <= X2 && y >= Y1 && y <= Y2;
}

#pragma region Symbols
Symbol Measurement::getSymbol()
{
	vector<Move> movements {};
	return Symbol(0, 0, movements);
}

Symbol Resistance::getSymbol()
{
	vector<Move> movements
	{
		Move(Right, 7),
		Move(Up),
		Move(Right),
		Move(Up),
		Move(Right),
		Move(Up),
		Move(Right),
		Move(Up),
		Move(Right),
		Move(Down),
		Move(Right),
		Move(Down),
		Move(Right),
		Move(Down),
		Move(Right),
		Move(Down),
		Move(Right, 7)
	};
	return Symbol(1, 4, movements);
}

Symbol Current::getSymbol()
{
	vector<Move> movements
	{
		Move(Right, 7)
	};
	return Symbol(1, 4, movements);
}
#pragma endregion Symbols
