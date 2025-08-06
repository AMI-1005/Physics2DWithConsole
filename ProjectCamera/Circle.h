#pragma once
#include <SDL3/SDL.h>
#include "Shape.h"
#include "globals.h"
#include <iostream> //debugging
#include "MemMaster.h"
/**
 * @class Circle
 * @brief Represents a drawable circle shape for the physics renderer.
 *
 * Inherits from Shape and implements the Render method to draw a circle
 * at a given position using the SDL renderer.
 */
class Circle : public Shape, virtual MemMaster
{
public:
    float radius; ///< The radius of the circle

    /**
     * @brief Construct a new Circle object with the given radius.
     * @param r The radius of the circle
     */
    Circle(float r) : radius(r) {}

    /**
     * @brief Render the circle at the given position using the SDL renderer.
     * @param position The center position of the circle
     * @param renderer The SDL renderer to use for drawing
     */
    void Render(const Vector<double>& position, SDL_Renderer* renderer) override
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
        int cx = static_cast<int>(position.x); // Center x
        int cy = static_cast<int>(position.y); // Center y
        int r = static_cast<int>(radius);      // Radius as int
        const int segments = 360;              // Number of segments for the circle
        for (int i = 0; i < segments; i++)
        {
            double theta = 2.0 * PI * i / segments; // Angle for this segment
            int x = static_cast<int>(cx + r * cos(theta)); // X coordinate
            int y = static_cast<int>(cy + r * sin(theta)); // Y coordinate
            SDL_RenderPoint(renderer, x, y); // Draw the point
        }
    }
};

