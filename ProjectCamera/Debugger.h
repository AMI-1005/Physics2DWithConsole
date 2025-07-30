#pragma once
#include "Body.h"
#include "World.h"
#include "globals.h" // Use renderer and window from here
#include"Properties.h"
#include <string>
#include <vector>
#include <deque>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

extern SDL_Renderer* renderer;
extern TTF_Font* gFont;

class Debugger
{
public:
    Debugger(World* world);

    // Call this every frame to handle input and render the chat/debug window
    void Update();

    // Call this to process SDL events (for text input)
    void HandleEvent(const SDL_Event& e);

    // Helper to render text (simple, for demonstration)
    void RenderText(const std::string& text, int x, int y, SDL_Color color);
    Properties* propertiesWindow = nullptr; // Pointer to properties window

    // Method to set properties window
    void inline SetPropertiesWindow(Properties* properties) {
        propertiesWindow = properties;
    }
    Properties* GetPropertiesWindow() const {
        return propertiesWindow;
    }
private:
    World* world;

    std::string inputBuffer;           // Current command being typed
    std::deque<std::string> chatLines; // Output lines to display
    bool inputActive = false;          // Is the input box active?

    // Helper to render the chat window at the bottom
    void RenderChatWindow();

    // Helper to process a command
    void ProcessCommand(const std::string& cmd);

    // Helper to get all body names from the world
    std::vector<std::string> GetBodyNames() const;
};

