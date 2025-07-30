#pragma once
#include "ConvexPolygon.h"
#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include "Body.h"
#include <SDL3_ttf/SDL_ttf.h>
#include"World.h"
class Properties
{
    // Private constructor to prevent direct instantiation
    Properties() = default;

public:
    ConvexPolygon propertiesWindow; ///< ConvexPolygon used for the window border
    Body* selectedBody = nullptr;   ///< Pointer to the currently selected body

    // Delete copy constructor and assignment operator
    Properties(const Properties&) = delete;
    Properties& operator=(const Properties&) = delete;

    // Static factory method to create an instance of Properties
    static Properties* CreateInstance() {
        return new Properties();
    }

    // Initialize the rectangle window polygon (4 points)
    void Init(const std::vector<Vector<double>>& points);

    // Set the selected body whose properties will be displayed
    void SetSelectedBody(Body* body) {
        selectedBody = body;
    }

    // Ensure selectedBody is set to the first element in world if not set, or if it is invalid
    void EnsureSelectedBody(World& world);

    // Render the rectangle and the properties of the selected object inside
    // Pass World by reference, and compute selected index from pointer
    void Render(SDL_Renderer* renderer, TTF_Font* font, World& world) {
        // Ensure selectedBody is always set to the first body if not set or if invalid
        if (!selectedBody || world.bodies.empty()) {
            if (!world.bodies.empty()) {
                selectedBody = world.bodies.front().get();
            } else {
                selectedBody = nullptr;
            }
        } else {
            // If selectedBody is not in world anymore, reset to first
            bool found = false;
            for (auto& b : world.bodies) {
                if (b.get() == selectedBody) {
                    found = true;
                    break;
                }
            }
            if (!found && !world.bodies.empty()) {
                selectedBody = world.bodies.front().get();
            } else if (!found) {
                selectedBody = nullptr;
            }
        }
        // Draw the rectangle window (filled background)
        if (propertiesWindow.vertices.size() == 4) {
            // Compute bounding box for fill
            double minX = propertiesWindow.vertices[0].x, maxX = propertiesWindow.vertices[0].x;
            double minY = propertiesWindow.vertices[0].y, maxY = propertiesWindow.vertices[0].y;
            for (const auto& v : propertiesWindow.vertices) {
                if (v.x < minX) minX = v.x;
                if (v.x > maxX) maxX = v.x;
                if (v.y < minY) minY = v.y;
                if (v.y > maxY) maxY = v.y;
            }
            SDL_FRect rect = { (float)minX, (float)minY, (float)(maxX - minX), (float)(maxY - minY) };
            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 220);
            SDL_RenderFillRect(renderer, &rect);
        }
        // Draw the border exactly along the vertices (no offset)
        propertiesWindow.Render(Vector<double>(0), renderer);
        if (!selectedBody || !font) return;
        // Draw properties as text inside the rectangle
        // Compute text start position relative to the bounding box
        double minX = propertiesWindow.vertices[0].x, minY = propertiesWindow.vertices[0].y;
        for (const auto& v : propertiesWindow.vertices) {
            if (v.x < minX) minX = v.x;
            if (v.y < minY) minY = v.y;
        }
        int x = static_cast<int>(minX) + 10, y = static_cast<int>(minY) + 10, lineHeight = 22;
        SDL_Color color = { 255, 255, 255, 255 };
        auto renderText = [&](const std::string& text, int tx, int ty) {
            SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), strlen(text.c_str()), color);
            if (!surface) return;
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!texture) { SDL_DestroySurface(surface); return; }
            SDL_FRect dst = { (float)tx, (float)ty, (float)surface->w, (float)surface->h };
            SDL_RenderTexture(renderer, texture, NULL, &dst);
            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        };
        renderText("Properties:", x, y); y += lineHeight;
        renderText("Position: (" + std::to_string(selectedBody->position.x) + ", " + std::to_string(selectedBody->position.y) + ")", x, y); y += lineHeight;
        renderText("Velocity: (" + std::to_string(selectedBody->velocity.x) + ", " + std::to_string(selectedBody->velocity.y) + ")", x, y); y += lineHeight;
        renderText("Mass: " + std::to_string(selectedBody->mass), x, y); y += lineHeight;
        renderText("Friction: " + std::to_string(selectedBody->coeff_friction), x, y); y += lineHeight;
        renderText("Restitution: " + std::to_string(selectedBody->coeff_restitution), x, y); y += lineHeight;
        // Compute selected body index
        int selectedIndex = 0, totalBodies = 0;
        for (auto it = world.bodies.begin(); it != world.bodies.end(); ++it, ++totalBodies) {
            if (it->get() == selectedBody) {
                selectedIndex = totalBodies;
            }
        }
        RenderBodyIndexSelector(renderer, font, selectedIndex, totalBodies, static_cast<int>(minX) + 10, static_cast<int>(minY) + 10 + lineHeight * 6);
    }

    // Draws a casket (box) with the index of the selected body and two arrows for navigation
    void RenderBodyIndexSelector(SDL_Renderer* renderer, TTF_Font* font, int selectedIndex, int totalBodies, int x, int y) {
        // Draw casket (rectangle)
        int boxWidth = 60, boxHeight = 40;
        SDL_FRect box = { (float)x, (float)y, (float)boxWidth, (float)boxHeight };
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 220);
        SDL_RenderFillRect(renderer, &box);
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderRect(renderer, &box);
        // Draw index number centered
        if (font) {
            std::string idxText = std::to_string(selectedIndex);
            SDL_Color color = {255,255,255,255};
            SDL_Surface* surface = TTF_RenderText_Solid(font, idxText.c_str(), idxText.size(), color);
            if (surface) {
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                if (texture) {
                    int textW = surface->w, textH = surface->h;
                    SDL_FRect dst = { (float)(x + (boxWidth-textW)/2), (float)(y + (boxHeight-textH)/2), (float)textW, (float)textH };
                    SDL_RenderTexture(renderer, texture, NULL, &dst);
                    SDL_DestroyTexture(texture);
                }
                SDL_DestroySurface(surface);
            }
        }
        // Draw left arrow
        int arrowY = y + boxHeight/2;
        int arrowSize = 12;
        SDL_SetRenderDrawColor(renderer, 180, 180, 0, 255);
        SDL_FPoint leftArrow[3] = {
            {(float)(x-10), (float)arrowY},
            {(float)(x-10+arrowSize), (float)(arrowY-arrowSize/2)},
            {(float)(x-10+arrowSize), (float)(arrowY+arrowSize/2)}
        };
        SDL_RenderLines(renderer, leftArrow, 3);
        // Draw right arrow
        SDL_FPoint rightArrow[3] = {
            {(float)(x+boxWidth+10), (float)arrowY},
            {(float)(x+boxWidth+10-arrowSize), (float)(arrowY-arrowSize/2)},
            {(float)(x+boxWidth+10-arrowSize), (float)(arrowY+arrowSize/2)}
        };
        SDL_RenderLines(renderer, rightArrow, 3);
    }

    // Handle mouse click for arrow navigation
    void HandleEvent(const SDL_Event& e, World& world);

    // Update logic for Properties (e.g., could be used for UI animation, polling, etc.)
    void Update(World& world);
};

