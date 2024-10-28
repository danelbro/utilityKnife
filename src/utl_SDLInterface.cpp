#include "utl_SDLInterface.hpp"

#include "utl_Box.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

namespace utl {

SdlException::SdlException(const std::string& message)
    : std::runtime_error{message}
{}

void sdl_deleter::operator()(SDL_Window* w) const
{
    LOG("destroying a window\n");
    SDL_DestroyWindow(w);
}

void sdl_deleter::operator()(SDL_Renderer* r) const
{
    LOG("destroying a renderer\n");
    SDL_DestroyRenderer(r);
}

void sdl_deleter::operator()(SDL_Surface* s) const
{
    LOG("destroying a surface\n");
    SDL_DestroySurface(s);
}

void sdl_deleter::operator()(SDL_Texture* t) const
{
    LOG("destroying a texture\n");
    SDL_DestroyTexture(t);
}

void sdl_deleter::operator()(TTF_Font* f) const
{
    LOG("closing a font\n");
    TTF_CloseFont(f);
}

bool init_SDL(uint32_t sdlFlags)
{
    if (!SDL_Init(sdlFlags)) {
        std::string sdlError{SDL_GetError()};
        throw SdlException(
            std::string{"Cannot initialise SDL! SDL_Error: " + sdlError});
    }

    if (!TTF_Init()) {
        std::string ttfError{SDL_GetError()};
        throw SdlException(
            std::string{"Cannot initialise SDL_TTF! TTF_Error: " + ttfError});
    }

    LOG("Initialised SDL\n");

    return true;
}

void quit_sdl()
{
    LOG("Quitting SDL\n");
    TTF_Quit();
    SDL_Quit();
}

Window::Window(SDL_Window* new_win) : m_winPtr{new_win, sdl_deleter()} {}

Window createWindow(const std::string& title, int w, int h, uint32_t flags)
{
    LOG("creating a window\n");

    SDL_Window* sdlWindow{SDL_CreateWindow(title.c_str(), w, h, flags)};

    if (!sdlWindow) {
        throw SdlException(
            std::string{"Cannot create window! SDL_Error: ", SDL_GetError()});
    }
    return Window{sdlWindow};
}

Renderer::Renderer(SDL_Renderer* new_rend) : m_rendPtr{new_rend, sdl_deleter()}
{}

WindowWithRenderer create_window_with_renderer(const std::string& title, int w,
                                               int h, uint32_t flags)
{
    LOG("Creating a window + renderer\n");

    SDL_Window* sdlWindow{nullptr};
    SDL_Renderer* sdlRenderer{nullptr};

    if (!SDL_CreateWindowAndRenderer(title.c_str(), w, h, flags, &sdlWindow,
                                     &sdlRenderer)) {
        throw SdlException(std::string{
            "Cannot create window/renderer! SDL_Error: ", SDL_GetError()});
    }

    return {Window{sdlWindow}, Renderer{sdlRenderer}};
}

Renderer createRenderer(const Window& window, const char* index)
{
    LOG("creating a renderer\n");

    SDL_Renderer* sdlRenderer{SDL_CreateRenderer(window.get(), index)};

    if (!sdlRenderer) {
        throw SdlException(
            std::string{"Cannot create renderer! SDL_Error: ", SDL_GetError()});
    }
    return Renderer{sdlRenderer};
}

bool Renderer::setVSync(int vsync)
{
    bool isVSyncSet{SDL_SetRenderVSync(m_rendPtr.get(), vsync)};
    if (!isVSyncSet) {
        throw SdlException(
            std::string{"Couldn't set vsync! SDL_Error: ", SDL_GetError()});
    }
    return isVSyncSet;
}

void clearScreen(Renderer& rend)
{
    SDL_RenderClear(rend.get());
}

void presentRenderer(Renderer& rend)
{
    SDL_RenderPresent(rend.get());
}

void setRendererDrawColour(Renderer& rend, const Colour& col)
{
    SDL_SetRenderDrawColor(rend.get(), col.r, col.g, col.b, col.a);
}

Colour getRendererDrawColour(const Renderer& rend)
{
    Colour col{};
    SDL_GetRenderDrawColor(rend.get(), &col.r, &col.g, &col.b, &col.a);
    return col;
}

void copyTexturePortion(Renderer& rend, Texture& tex, Rect& srcRect,
                        Rect& dstRect)
{
    SDL_RenderTexture(rend.get(), tex.get(), srcRect.get(), dstRect.get());
}

void drawPoint(Renderer& rend, int x, int y)
{
    SDL_RenderPoint(rend.get(), static_cast<float>(x), static_cast<float>(y));
}

Surface::Surface(SDL_Surface* new_surf) : m_surfPtr{new_surf, sdl_deleter()} {}

Texture::Texture(SDL_Texture* new_tex) : m_texPtr{new_tex, sdl_deleter()} {}

textureAndSize::textureAndSize(Texture&& newTexP, int newW, int newH)
    : texP{std::move(newTexP)}, w{newW}, h{newH}
{}

textureAndSize createTextTexture(Font& font, const std::string& text,
                                 const Colour& text_colour, Renderer& rend)
{
    LOG("creating a surface\n");
    SDL_Surface* textSurface{TTF_RenderText_Blended(
        font.get(), text.c_str(), text.length(), text_colour)};

    if (!textSurface) {
        throw SdlException(
            std::string{"Cannot create surface! SDL_Error: ", SDL_GetError()});
    }

    int w{textSurface->w};
    int h{textSurface->h};

    LOG("creating a texture from the surface\n");
    SDL_Texture* textTexture{
        SDL_CreateTextureFromSurface(rend.get(), textSurface)};

    if (!textTexture) {
        ERRLOG("%s\n", SDL_GetError());
        throw SdlException("Could not create texture!");
    }

    LOG("destroying the surface\n");
    SDL_DestroySurface(textSurface);

    return textureAndSize{textTexture, w, h};
}

Font::Font(TTF_Font* new_font) : m_fontPtr{new_font, sdl_deleter()} {}

Font createFont(const std::filesystem::path& path, int font_size)
{
    LOG("creating a font\n");

    TTF_Font* font{
        TTF_OpenFont(path.string().c_str(), static_cast<float>(font_size))};

    if (!font) {
        throw SdlException(
            std::string{"Failed to make font! TTF_Error: ", SDL_GetError()});
    }
    return Font{font};
}

Rect::Rect(SDL_FRect* new_rect) : m_rectPtr{new_rect} {}

Rect::Rect(int x, int y, int w, int h)
    : m_rectPtr{std::make_unique<SDL_FRect>(
        static_cast<float>(x), static_cast<float>(y), static_cast<float>(w),
        static_cast<float>(h))}
{}

void process_input(Box& screen, uint32_t windowID,
                   std::array<bool, KeyFlag::K_TOTAL>& key_state)
{
    SDL_Event ev{};
    key_state[KeyFlag::WINDOW_CHANGE] = false;

    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_EVENT_QUIT) {
            key_state[KeyFlag::QUIT] = true;
        }

        else if (ev.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED) {
            if (ev.window.windowID == windowID) {
                screen.w = ev.window.data1;
                screen.h = ev.window.data2;
                key_state[KeyFlag::WINDOW_CHANGE] = true;
            }
        }

        else if (ev.type == SDL_EVENT_KEY_DOWN) {
            switch (ev.key.key) {
            case SDLK_ESCAPE:
                key_state[KeyFlag::K_ESCAPE] = true;
                break;
            case SDLK_LEFT:
                key_state[KeyFlag::K_LEFT] = true;
                break;
            case SDLK_RIGHT:
                key_state[KeyFlag::K_RIGHT] = true;
                break;
            case SDLK_UP:
                key_state[KeyFlag::K_UP] = true;
                break;
            case SDLK_DOWN:
                key_state[KeyFlag::K_DOWN] = true;
                break;
            case SDLK_SPACE:
                key_state[KeyFlag::K_SPACE] = true;
                break;
            case SDLK_RETURN:
                key_state[KeyFlag::K_ENTER] = true;
                break;
            case SDLK_LSHIFT:
                key_state[KeyFlag::K_LSHIFT] = true;
                break;
            default:
                break;
            }
        } else if (ev.type == SDL_EVENT_KEY_UP) {
            switch (ev.key.key) {
            case SDLK_ESCAPE:
                key_state[KeyFlag::K_ESCAPE] = false;
                break;
            case SDLK_LEFT:
                key_state[KeyFlag::K_LEFT] = false;
                break;
            case SDLK_RIGHT:
                key_state[KeyFlag::K_RIGHT] = false;
                break;
            case SDLK_UP:
                key_state[KeyFlag::K_UP] = false;
                break;
            case SDLK_DOWN:
                key_state[KeyFlag::K_DOWN] = false;
                break;
            case SDLK_SPACE:
                key_state[KeyFlag::K_SPACE] = false;
                break;
            case SDLK_LSHIFT:
                key_state[KeyFlag::K_LSHIFT] = false;
                break;
            default:
                break;
            }
        }
    }
}

}  // namespace utl
