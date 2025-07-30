#include "Properties.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

void Properties::Init(const std::vector<Vector<double>>& points) {
    propertiesWindow = ConvexPolygon(points);
}

// Ensure selectedBody is set to the first element in world if not set
void Properties::EnsureSelectedBody(World& world) {
    if (!selectedBody) {
        if (!world.bodies.empty()) {
            selectedBody = world.bodies.front().get();
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
}

void Properties::Update(World& world) {
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
}

void Properties::HandleEvent(const SDL_Event& e, World& world) {
    if (!selectedBody || world.bodies.empty() || e.type != SDL_EVENT_MOUSE_BUTTON_DOWN)
        return;
    // Compute bounding box for the selector
    double minX = propertiesWindow.vertices[0].x, minY = propertiesWindow.vertices[0].y;
    for (const auto& v : propertiesWindow.vertices) {
        if (v.x < minX) minX = v.x;
        if (v.y < minY) minY = v.y;
    }
    int x = static_cast<int>(minX) + 10, y = static_cast<int>(minY) + 10 + 22 * 6;
    int boxWidth = 60, boxHeight = 40;
    int arrowSize = 12;
    int arrowY = y + boxHeight / 2;
    int mouseX = e.button.x, mouseY = e.button.y;
    // Left arrow bounding box
    SDL_Rect leftArrowRect = { x-10, arrowY-arrowSize/2, arrowSize, arrowSize };
    // Right arrow bounding box
    SDL_Rect rightArrowRect = { x+boxWidth+10-arrowSize, arrowY-arrowSize/2, arrowSize, arrowSize };
    // Find current index
    int selectedIndex = 0, totalBodies = 0;
    for (auto it = world.bodies.begin(); it != world.bodies.end(); ++it, ++totalBodies) {
        if (it->get() == selectedBody) {
            selectedIndex = totalBodies;
        }
    }
    // Left arrow click
    if (mouseX >= leftArrowRect.x && mouseX <= leftArrowRect.x+leftArrowRect.w &&
        mouseY >= leftArrowRect.y && mouseY <= leftArrowRect.y+leftArrowRect.h) {
        if (totalBodies > 0) {
            int newIndex = (selectedIndex - 1 + totalBodies) % totalBodies;
            auto it = world.bodies.begin();
            std::advance(it, newIndex);
            selectedBody = it->get();
        }
    }
    // Right arrow click
    if (mouseX >= rightArrowRect.x && mouseX <= rightArrowRect.x+rightArrowRect.w &&
        mouseY >= rightArrowRect.y && mouseY <= rightArrowRect.y+rightArrowRect.h) {
        if (totalBodies > 0) {
            int newIndex = (selectedIndex + 1) % totalBodies;
            auto it = world.bodies.begin();
            std::advance(it, newIndex);
            selectedBody = it->get();
        }
    }
}
