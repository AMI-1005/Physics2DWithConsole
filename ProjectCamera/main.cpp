/**
 * @file main.cpp
 * @brief Entry point for the Physics Renderer application.
 *
 * Initializes SDL, creates the main window and renderer, sets up the world and debugger,
 * and runs the main event loop for simulation and rendering.
 */
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "Body.h"
#include "World.h"
#include "Shape.h"
#include "Vector.h"
#include "Circle.h"
#include "Debugger.h"
#include "Properties.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// Global SDL objects and font
SDL_Window* window = nullptr;         // Main application window
SDL_Renderer* renderer = nullptr;     // SDL renderer
TTF_Font* gFont = nullptr;            // Global font
SDL_Window* gWindow = nullptr;        // Global window pointer for debugger

/**
 * @brief Main function for the Physics Renderer application.
 *
 * Initializes SDL and SDL_ttf, creates the main window and renderer, sets up the world and debugger,
 * and runs the main event loop for simulation and rendering.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Application exit code.
 */
int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
        
    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        SDL_Log("Couldn't initialize SDL_ttf");
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    // Load font (change path as needed)
    gFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 16);
    if (!gFont) {
        SDL_Log("Couldn't load font");
        TTF_Quit();
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    // Create window
    window = SDL_CreateWindow("Physics Renderer", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    gWindow = window; // Assign to global pointer for debugger
    if (!window) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        TTF_CloseFont(gFont);
        TTF_Quit();
        return SDL_APP_FAILURE;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Couldn't create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(gFont);
        TTF_Quit();
        return SDL_APP_FAILURE;
    }

    // Create world, debugger and properties window
    World world;
    Debugger debugger(&world);

    // Use the factory method to create an instance of Properties
    Properties* propertiesWindow = Properties::CreateInstance();
    if (propertiesWindow) {
        debugger.SetPropertiesWindow(propertiesWindow);
        // Use floating point math for correct placement
        propertiesWindow->Init({
            []{ Vector<double> v(2); v.set((2.0/3.0)*WINDOW_WIDTH, (1.0/3.0)*WINDOW_HEIGHT); return v; }(),
            []{ Vector<double> v(2); v.set((2.0/3.0)*WINDOW_WIDTH, (2.0 / 3.0)*WINDOW_HEIGHT); return v; }(),
            []{ Vector<double> v(2); v.set(WINDOW_WIDTH, (2.0 / 3.0)*WINDOW_HEIGHT); return v; }(),
            []{ Vector<double> v(2); v.set(WINDOW_WIDTH, (1.0/3.0)*WINDOW_HEIGHT); return v; }()
			});
    }
    // Add a circle object to the world
    //world.AddBody(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0, 0, 5, 0, new Circle(50));

    bool running = true;
    SDL_Event event;
    int lastTime = SDL_GetTicks();
    double deltaTime = 0.0;

    // Main event loop
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            debugger.HandleEvent(event);
        }

        // Update world timing
        int currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0;
        lastTime = currentTime;

        // Update simulation
        world.Update(deltaTime);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render world and debugger overlay
        world.Render(renderer);
        debugger.Update();

        // Present the rendered frame
        SDL_RenderPresent(renderer);
    }

    // Cleanup resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(gFont);
    TTF_Quit();
    SDL_Quit();
    return 0;
}