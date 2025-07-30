// World.h
// Manages all physics bodies and simulation logic for the world.
#pragma once
#include <list>
#include <memory>
#include "Body.h"
#include "Shape.h"

// The World class manages all physics bodies and simulation logic.
class World
{
public:
    // List of all bodies in the world. Each body is owned by a unique_ptr.
    std::list<std::unique_ptr<Body>> bodies;

    // Update all bodies in the world for the given time step.
    void Update(double deltaTime);

    // Render all bodies in the world using the given SDL renderer.
    void Render(SDL_Renderer* renderer);

    // Add a new body to the world with position, velocity, force, and shape.
    void AddBody(double positionX, double positionY, double velocityX,
        double velocityY, double initialForceX, double initialForceY, Shape* shp);

    // Remove a body from the world by its index in the list.
    void RemoveBody(size_t index);

    // Remove all bodies from the world.
    void ClearBodies();
};
