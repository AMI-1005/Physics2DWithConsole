#include "Body.h"
#include <SDL3/SDL.h>
#include <iostream> //debugging

/**
 * @brief Construct a new Body object with initial state.
 * @param pos Initial position vector
 * @param vel Initial velocity vector
 * @param m Mass of the body
 * @param force Initial force vector
 */
Body::Body(const Vector<double>& pos,
    const Vector<double>& vel,
    double m, const Vector<double>& force)
    : position(pos)
    , velocity(vel)
    , acceleration(0)   // 0-dim vector
    , gravity(2)        // 2-dim vector
    , angular_acc(2)    // 2-dim vector`
    , angular_vel(2)    // 2-dim vector
    , rotation(2)       // 2-dim vector
    , mass(m)
    , inertia(1.0)
    , coeff_friction(0.5)
    , coeff_restitution(0.5)
    , force(force)
    , torque(0.0)
{
    // All physical properties initialized
}

/**
 * @brief Update the body's physics state for the given time step.
 *
 * Updates acceleration, velocity, position, angular acceleration,
 * angular velocity, and rotation based on current forces and torque.
 * @param deltaTime Time step for the update
 */
void Body::Update(double deltaTime)
{
    // Linear motion update
    // drag_force = -liniar_drag * velocity
    Vector<double> drag_force = liniar_drag * (-1.0);
    drag_force = drag_force * velocity.x; // Only x component, or use dotProduct if needed
    acceleration = (force + drag_force) / mass + gravity;
    velocity = velocity + acceleration * deltaTime;
    position = position + velocity * deltaTime;
    //velocity = velocity + impulse / mass;
    impulse = Vector<double>(0); // clear after application

    // Angular motion update
    // drag_torque = -angular_drag * angular_vel
    Vector<double> drag_torque = angular_drag * (-1.0);
    drag_torque = drag_torque * angular_vel.x; // Only x component, or use dotProduct if needed
    angular_acc = (torque + drag_torque.x) / inertia; // Use x component for scalar torque
    angular_vel = angular_vel + angular_acc * deltaTime;
    rotation = rotation + angular_vel * deltaTime;
}

/**
 * @brief Render the body using the given SDL renderer.
 *
 * Calls Render on all attached shapes, passing the current position.
 * @param renderer SDL renderer to use
 */
void Body::Render(SDL_Renderer* renderer)
{
    for (auto& shapePtr : shapes)
        shapePtr->Render(position, renderer);
}
