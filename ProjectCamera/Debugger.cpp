/**
 * Debugger.cpp
 *
 * Implements a simple in-game chat/debugger window for interacting with the physics world.
 * Allows listing, adding, and modifying bodies, and provides a help command.
 *
 * Controls:
 *   - Press ` (backtick) to toggle chat input
 *   - Press ESC to close chat input
 *   - Enter commands when chat is active
 *
 * Commands:
 *   - help: Show available commands
 *   - list: List all bodies
 *   - add [x y vx vy fx fy]: Add a new body (all arguments optional)
 *   - set <index> <property> <value>: Set a property of a body by index
 */
#include "Debugger.h"
#include "globals.h"
#include "Circle.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <map>

// External SDL objects and font
extern SDL_Renderer* renderer;
extern TTF_Font* gFont;
extern SDL_Window* gWindow;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// Use the macros for window size
#define CHAT_WINDOW_WIDTH WINDOW_WIDTH
#define CHAT_WINDOW_HEIGHT WINDOW_HEIGHT

// Scroll offset for chat window (how many lines up from the bottom)
static int chatScrollOffset = 0;
// Flag to control chat window visibility
static bool chatVisible = true;

/**
 * @brief Handles all SDL events related to the chat/debugger window.
 *        Includes toggling, scrolling, and closing the chat window.
 * @param e The SDL_Event to process
 */
void Debugger::HandleEvent(const SDL_Event& e)
{
    // Toggle chat input with backtick (`) key
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.scancode == SDL_SCANCODE_GRAVE) {
        if (!chatVisible) {
            chatVisible = true; // Show chat if hidden
        }
        inputActive = !inputActive; // Toggle input mode
        if (inputActive) {
            SDL_StartTextInput(gWindow); // Start text input
        } else {
            SDL_StopTextInput(gWindow); // Stop text input
        }
        return;
    }
    // Scroll chat window with up/down arrow keys (when not typing)
    if (chatVisible && !inputActive && e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.scancode == SDL_SCANCODE_UP) {
            if (chatScrollOffset + 1 < (int)chatLines.size())
                ++chatScrollOffset; // Scroll up
        } else if (e.key.scancode == SDL_SCANCODE_DOWN) {
            if (chatScrollOffset > 0)
                --chatScrollOffset; // Scroll down
        }
        return;
    }
    // Scroll chat window with mouse wheel (when not typing)
    if (chatVisible && !inputActive && e.type == SDL_EVENT_MOUSE_WHEEL) {
        if (e.wheel.y > 0) { // scroll up
            if (chatScrollOffset + 1 < (int)chatLines.size())
                ++chatScrollOffset;
        } else if (e.wheel.y < 0) { // scroll down
            if (chatScrollOffset > 0)
                --chatScrollOffset;
        }
        return;
    }
    // Pass mouse events to properties window for arrow navigation
    if (propertiesWindow && world) {
        propertiesWindow->HandleEvent(e, *world);
    }
    // Close chat with Escape (even if not typing)
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.scancode == SDL_SCANCODE_ESCAPE) {
        if (inputActive) {
            inputActive = false;
            SDL_StopTextInput(gWindow); // Stop text input if active
        }
        chatVisible = false; // Hide chat window from view
        return;
    }
    // Handle text input and editing when chat is active
    if (chatVisible && e.type == SDL_EVENT_KEY_DOWN && inputActive) {
        if (e.key.key == SDLK_RETURN || e.key.key == SDLK_KP_ENTER) {
            if (!inputBuffer.empty()) {
                ProcessCommand(inputBuffer); // Process entered command
                inputBuffer.clear();
            }
        } else if (e.key.key == SDLK_BACKSPACE && !inputBuffer.empty()) {
            inputBuffer.pop_back(); // Remove last character
        }
    } else if (chatVisible && e.type == SDL_EVENT_TEXT_INPUT && inputActive) {
        inputBuffer += e.text.text; // Append typed character(s)
    }
}

/**
 * Helper function to trim whitespace from both ends of a string.
 * @param s The string to trim
 * @return Trimmed string
 */
static std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\n\r");
    auto end = s.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

/**
 * Helper to set a component (x or y) of a Vector2D.
 * @param v The vector to modify
 * @param idx 0 for x, 1 for y
 * @param value The value to set
 */
static void setVec2Component(Vector<double>& v, int idx, double value) {
    if (idx == 0) v.x = value;
    else if (idx == 1) v.y = value;
    v.syncComponents();
}

/**
 * Debugger constructor.
 * @param world Pointer to the physics world
 */
Debugger::Debugger(World* world)
    : world(world)
{
    inputActive = true; // Start with chat input active
    chatLines.push_back("Debugger ready. Type 'list' to see all bodies.");
}

/**
 * Call every frame to render the chat window.
 */
void Debugger::Update()
{
    RenderChatWindow();
}

/**
 * Renders a single line of text at the given position.
 * @param text The text to render
 * @param x X position
 * @param y Y position
 * @param color Text color
 */
void Debugger::RenderText(const std::string& text, int x, int y, SDL_Color color)
{
    if (!gFont) return;
    SDL_Surface* surface = TTF_RenderText_Solid(gFont, text.c_str(), 0, color); // Render text to surface
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Create texture from surface
    if (!texture) { SDL_DestroySurface(surface); return; }
    SDL_FRect dst = { (float)x, (float)y, (float)surface->w, (float)surface->h };
    SDL_RenderTexture(renderer, texture, NULL, &dst); // Draw texture
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

/**
 * Renders the chat window and recent chat lines at the bottom of the screen.
 * Only renders if chatVisible is true.
 */
void Debugger::RenderChatWindow()
{
    if (propertiesWindow && world) {
        propertiesWindow->Render(renderer, gFont, *world); // Pass world by reference
    }
    if (!chatVisible) return; // Do not render chat if hidden
    int chatHeight = 120;
    int x = 0, y = WINDOW_HEIGHT - chatHeight, w = WINDOW_WIDTH, h = chatHeight;
    SDL_FRect chatRect = { (float)x, (float)y, (float)w, (float)h };
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 200); // Chat background
    SDL_RenderFillRect(renderer, &chatRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White line
    SDL_RenderLine(renderer, x, y + h - 30, x + w, y + h - 30);
    int lineY = y + 10;
    int lineNum = 0;
    // Show up to 3 lines, starting from scroll offset
    for (auto it = chatLines.rbegin() + chatScrollOffset; it != chatLines.rend() && lineNum < 3; ++it, ++lineNum) {
        RenderText(*it, x + 10, lineY, SDL_Color{255,255,255,255}); // Render chat line
        lineY += 25;
    }
    RenderText(inputBuffer, x + 10, y + h - 25, SDL_Color{100,255,100,255}); // Render input buffer
}

/**
 * Processes a command entered in the chat window.
 * @param cmd The command string
 */
void Debugger::ProcessCommand(const std::string& cmd)
{
    std::cout << "Command entered: " << cmd << std::endl;
    std::istringstream iss(cmd);
    std::string command;
    iss >> command;
    // Handle 'help' command
    if (command == "help") {
        chatLines.push_back("Commands:");
        chatLines.push_back("list - List all bodies");
        chatLines.push_back("add [x y vx vy fx fy] - Add a body");
        chatLines.push_back("set <index> <property> <value> - Set property of body");
        chatLines.push_back("help - Show this help");
        chatLines.push_back("Press ESC to close chat");
    } else if (command == "list") {
        // List all bodies in the world
        int idx = 0;
        for (const auto& body : world->bodies) {
            chatLines.push_back("Body " + std::to_string(idx++));
        }
        if (idx == 0) chatLines.push_back("No bodies in world.");
    } else if (command == "add") {
        // Parse arguments: add [x] [y] [vx] [vy] [fx] [fy] (all optional)
        double x = 100 + 20 * (int)world->bodies.size();
        double y = 200, vx = 0, vy = 0, fx = 0, fy = 0;
        if (iss >> x) {
            if (iss >> y) {
                if (iss >> vx) {
                    if (iss >> vy) {
                        if (iss >> fx) {
                            iss >> fy;
                        }
                    }
                }
            }
        }
        world->AddBody(x, y, vx, vy, fx, fy, new Circle(20));
        chatLines.push_back("Added a new circle body at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    } else if (command == "set") {
        // Set a property of a body by index
        int idx;
        std::string prop;
        double value;
        if (iss >> idx >> prop >> value) {
            auto it = world->bodies.begin();
            std::advance(it, idx);
            if (it != world->bodies.end()) {
                Body* body = it->get();
                if (prop == "x") setVec2Component(body->position, 0, value);
                else if (prop == "y") setVec2Component(body->position, 1, value);
                else if (prop == "vx") setVec2Component(body->velocity, 0, value);
                else if (prop == "vy") setVec2Component(body->velocity, 1, value);
                else if (prop == "fx") setVec2Component(body->force, 0, value);
                else if (prop == "fy") setVec2Component(body->force, 1, value);
                else if (prop == "mass") body->mass = value;
                else if (prop == "inertia") body->inertia = value;
                else if (prop == "friction") body->coeff_friction = value;
                else if (prop == "restitution") body->coeff_restitution = value;
                else {
                    chatLines.push_back("Unknown property: " + prop);
                    return;
                }
                chatLines.push_back("Set body " + std::to_string(idx) + " " + prop + " to " + std::to_string(value));
            } else {
                chatLines.push_back("Body index out of range");
            }
        } else {
            chatLines.push_back("Usage: set <index> <property> <value>");
        }
    } else {
        // Unknown command
        chatLines.push_back("Unknown command: " + cmd);
    }
    // Keep more history for scrolling
    while (chatLines.size() > 50) chatLines.pop_front();
    chatScrollOffset = 0; // Reset scroll to bottom on new command
}
