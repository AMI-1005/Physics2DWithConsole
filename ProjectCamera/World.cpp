// World.cpp
// Implements the World class, which manages all physics bodies and simulation logic.
#include<memory>
#include <initializer_list>
#include <SDL3/SDL.h>
#include "World.h"
#include "Vector.h"  // for Zero()
#include "Shape.h"

// Update all bodies in the world for the given time step.
void World::Update(double deltaTime)
{
    for (auto& bodyPtr : bodies)
    {
        bodyPtr->Update(deltaTime); // Update physics for each body
        bodyPtr->force = Vector<double>::Zero(2); // Reset force after update
        bodyPtr->torque = 0; // Reset torque after update
    }
}

// Render all bodies in the world using the given SDL renderer.
void World::Render(SDL_Renderer* renderer)
{
    for (auto& bodyPtr : bodies)
    {
        bodyPtr->Render(renderer); // Render each body
    }
}

// Add a new body to the world with position, velocity, force, and shape.
void World::AddBody(double positionX, double positionY, double velocityX,
    double velocityY, double initialForceX, double initialForceY, Shape* shp)
{
    Vector<double> pos(2);
    pos.set(positionX, positionY);
    Vector<double> vel(2);
    vel.set(velocityX, velocityY);
    Vector<double> force(2);
    force.set(initialForceX, initialForceY);

    auto bodyPtr = std::make_unique<Body>(pos, vel, 0.1, force); // Create new body
    bodyPtr->shapes.push_back(std::unique_ptr<Shape>(shp)); // Attach shape to body
    bodies.push_back(std::move(bodyPtr)); // Add body to world
}

// Remove a body from the world by its index in the list.
void World::RemoveBody(size_t index)
{
    if (index < bodies.size())
    {
        auto it = bodies.begin();
        std::advance(it, index);
        bodies.erase(it); // Remove body at the given index
    }
}

// Remove all bodies from the world.
void World::ClearBodies()
{
    bodies.clear();
}