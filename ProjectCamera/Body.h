#pragma once
#include <list>
#include <memory>
#include "Vector.h"
#include "Shape.h"
#pragma warning(disable : 4244)

/**
 * @class Body
 * @brief Represents a physical object in the simulation.
 *
 * Stores position, velocity, acceleration, mass, force, and shape(s).
 * Handles physics updates and rendering for the object.
 */
class Body
{
public:
    std::list<std::unique_ptr<Shape>> shapes; ///< List of shapes attached to this body

    /**
     * @brief Construct a new Body object.
     * @param pos Initial position vector
     * @param vel Initial velocity vector
     * @param m Mass of the body
     * @param force Initial force vector
     */
    Body(const Vector<double>& pos,
        const Vector<double>& vel,
        double m, const Vector<double>& force );

    Vector<double> position;      ///< Position vector
    Vector<double> velocity;      ///< Velocity vector
    Vector<double> acceleration;  ///< Acceleration vector
    Vector<double> gravity;       ///< Gravity vector
    Vector<double> angular_acc;   ///< Angular acceleration vector
    Vector<double> angular_vel;   ///< Angular velocity vector
    Vector<double> rotation;      ///< Rotation vector

    double mass;                  ///< Mass of the body
    double inertia;               ///< Moment of inertia
    double coeff_friction;        ///< Coefficient of friction
    double coeff_restitution;     ///< Coefficient of restitution

    Vector<double> force;         ///< Force vector
    double torque;                ///< Torque value
	Vector<double> normal;       ///< Normal vector for collision response
	Vector<double> impulse;       ///< Impulse vector for collision response
	Vector<double> liniar_drag; ///< Linear drag vector
	Vector<double> angular_drag; ///< Angular drag vector
	Vector<double> center_of_mass; ///< Center of mass vector


    /**
     * @brief Update the body's physics state for the given time step.
     * @param deltaTime Time step for the update
     */
    void Update(double deltaTime);

    /**
     * @brief Render the body using the given SDL renderer.
     * @param renderer SDL renderer to use
     */
    void Render(SDL_Renderer* renderer);
};
