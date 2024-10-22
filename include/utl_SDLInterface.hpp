#pragma once

/**
 * Interface wrapper functions and data for SDL3.
 */

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>

struct Box;

namespace utl {

struct Colour {
    std::uint8_t r{};
    std::uint8_t g{};
    std::uint8_t b{};
    std::uint8_t a{};

    operator SDL_Color() const { return {r, g, b, a}; }
};

// Custom deleters for SDL types. Pass when constructing a unique_ptr
// thanks to https://stackoverflow.com/a/24252225
struct sdl_deleter {
    void operator()(SDL_Window*) const;
    void operator()(SDL_Renderer*) const;
    void operator()(SDL_Surface*) const;
    void operator()(SDL_Texture*) const;
    void operator()(TTF_Font*) const;
};

// wrapper around std::runtime_error to make SDL exception handling smoother
class SdlException : public std::runtime_error {
public:
    SdlException(const std::string& e);
};

// Initialise SDL with sdlFlags.
// Throw SdlException if initialisation fails
void init(uint32_t sdlFlags);

// Run SDL and TTF quit functions
void quit_sdl();

struct Window {
public:
    Window(SDL_Window*);

    uint32_t ID() { return SDL_GetWindowID(m_winPtr.get()); }
    SDL_Window* get() { return m_winPtr.get(); }

private:
    std::unique_ptr<SDL_Window, sdl_deleter> m_winPtr;
};

struct Renderer {
public:
    Renderer(SDL_Renderer*);

    SDL_Renderer* get() { return m_rendPtr.get(); }
    bool setVSync(int vsync);

private:
    std::unique_ptr<SDL_Renderer, sdl_deleter> m_rendPtr;
};

struct Surface {
public:
    Surface(SDL_Surface*);

    SDL_Surface* get() { return m_surfPtr.get(); }

private:
    std::unique_ptr<SDL_Surface, sdl_deleter> m_surfPtr;
};

struct Texture {
public:
    Texture(SDL_Texture*);

    SDL_Texture* get() { return m_texPtr.get(); }
    void reset(SDL_Texture* new_ptr) { m_texPtr.reset(new_ptr); }

private:
    std::unique_ptr<SDL_Texture, sdl_deleter> m_texPtr;
};

struct Font {
public:
    Font(TTF_Font*);

    TTF_Font* get() { return m_fontPtr.get(); }

private:
    std::unique_ptr<TTF_Font, sdl_deleter> m_fontPtr;
};

struct Rect {
public:
    Rect(SDL_FRect*);
    Rect(int x, int y, int w, int h);

    SDL_FRect* get() { return m_rectPtr.get(); }

private:
    std::unique_ptr<SDL_FRect> m_rectPtr;
};

// Create an SDL_Window*. Throw an SdlException if creation fails
Window createWindow(const std::string& title, int w, int h, Uint32 flags);

// Create an SDL_Renderer*. Throw an SdlException if creation fails
Renderer createRenderer(Window& window, const char* index);

void clearScreen(Renderer&);
void presentRenderer(Renderer&);
void setRendererDrawColour(Renderer&, const Colour&);
Colour getRendererDrawColour(Renderer&);
void copyTexturePortion(Renderer&, Texture&, Rect& src, Rect& dst);
void drawPoint(Renderer&, int x, int y);

struct textureAndSize {
    textureAndSize(Texture newTexP, int newW, int newH);

    Texture texP;
    int w;
    int h;
};

// Create an SDL_Texture* rendered from text.
// Throw an SdlException if creation fails
textureAndSize createTextTexture(Font& font, const std::string& text,
                                 const Colour& text_colour, Renderer& rend);

// Create a TTF_Font. Throw an SdlException if creation fails
Font createFont(const std::filesystem::path& path, int font_size);

enum KeyFlag
{
    K_LEFT,
    K_RIGHT,
    K_UP,
    K_DOWN,
    K_SPACE,
    K_ENTER,
    K_LSHIFT,
    K_ESCAPE,
    QUIT,
    WINDOW_CHANGE,
    K_TOTAL
};

// process SDL input into a more friendly form. Also deals with
// window resizing
void process_input(Box& screen, uint32_t windowID,
                   std::array<bool, KeyFlag::K_TOTAL>& key_state);

}  // namespace utl
