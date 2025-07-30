#pragma once
#include "Shape.h"
#include "Vector.h"
#include <SDL3/SDL.h>
#include <vector>

/**
 * @class ConvexPolygon
 * @brief Represents a convex polygon shape for rendering and physics.
 *
 * Stores vertices and normals as vectors. Inherits from Shape.
 */
class ConvexPolygon : public Shape
{
public:
    std::vector<Vector<double>> vertices; ///< Vertices of the polygon
    std::vector<Vector<double>> normals;  ///< Normals for each edge
    
    // Default constructor
    ConvexPolygon() = default;

    /**
     * @brief Construct a new ConvexPolygon object with given node positions.
     * @param points The positions of the polygon's vertices (in local space)
     */
    ConvexPolygon(const std::vector<Vector<double>>& points);

    /**
     * @brief Render the convex polygon at the given position using the SDL renderer.
     * @param position The position to render at (offset for all vertices)
     * @param renderer The SDL renderer to use
     */
    void Render(const Vector<double>& position, SDL_Renderer* renderer) override;
};

// Implementation of constructor
inline ConvexPolygon::ConvexPolygon(const std::vector<Vector<double>>& points)
    : vertices(points), normals(points.size()) {}

// Implementation of Render function
inline void ConvexPolygon::Render(const Vector<double>& position, SDL_Renderer* renderer)
{   
    if (vertices.size() < 2) return; // Need at least 2 points to draw
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Vector<double>& v1 = vertices[i];
        const Vector<double>& v2 = vertices[(i + 1) % vertices.size()]; // wrap around
        int x1 = static_cast<int>(position.x + v1.x);
        int y1 = static_cast<int>(position.y + v1.y);
        int x2 = static_cast<int>(position.x + v2.x);
        int y2 = static_cast<int>(position.y + v2.y);
        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}