#pragma once
#include "Vector.h"
#include <SDL3/SDL.h>
class Shape
{
public:
	virtual void Render(const Vector<double>& position, SDL_Renderer* renderer) = 0;
};

