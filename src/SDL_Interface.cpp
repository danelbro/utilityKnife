#include "SDL_Interface.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "GameWorld.hpp"

namespace utl {
    SdlException::SdlException(const std::string& message)
        : std::runtime_error{ message }
    {}

    void init(uint32_t sdlFlags)
    {
        if (SDL_Init(sdlFlags) != 0)
            throw SdlException(
                std::string{ "Cannot initialise SDL! SDL_Error: ",
                SDL_GetError() });

        if (TTF_Init() == -1)
            throw SdlException(
                std::string{ "Cannot initialise SDL_TTF! TTF_Error: ",
                TTF_GetError() });
    }

    void quit_sdl()
    {
        TTF_Quit();
        SDL_Quit();
    }

    // Window creation

    Window::Window(SDL_Window* new_win)
        : m_winPtr{ new_win, sdl_deleter() }
    {}

    Window createWindow(const std::string& title, int x, int y, int w, int h,
        uint32_t flags)
    {
#ifdef _DEBUG
        errorLogger << "creating a window\n";
#endif
        Window window{ SDL_CreateWindow(title.c_str(), x,y, w, h, flags) };

        if (!window.get())
            throw SdlException(
                std::string{ "Cannot create window! SDL_Error: ",
                                SDL_GetError() });

        return window;
    }

    // Renderer creation

    Renderer::Renderer(SDL_Renderer* new_rend)
        : m_rendPtr{ new_rend, sdl_deleter() }
    {}

    Renderer createRenderer(Window& window, int index, uint32_t flags)
    {
#ifdef _DEBUG
        errorLogger << "creating a renderer\n";
#endif
        Renderer rend{ SDL_CreateRenderer(window.get(), index, flags) };

        if (!rend.get())
            throw SdlException(
                std::string{ "Cannot create renderer! SDL_Error: ",
                SDL_GetError() });

        return rend;
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

    Colour getRendererDrawColour(Renderer& rend)
    {
        Colour col{};
        SDL_GetRenderDrawColor(rend.get(), &col.r, &col.g, &col.b, &col.a);
        return col;
    }

    void copyTexturePortion(Renderer& rend, Texture& tex,
                            Rect& srcRect, Rect& dstRect)
    {
        SDL_RenderCopy(rend.get(), tex.get(), srcRect.get(), dstRect.get());
    }

    void drawPoint(Renderer& rend, int x, int y)
    {
        SDL_RenderDrawPoint(rend.get(), x, y);
    }

    // Surface creation

    Surface::Surface(SDL_Surface* new_surf)
        : m_surfPtr{ new_surf, sdl_deleter() }
    {}

    // Texture creation

    Texture::Texture(SDL_Texture* new_tex)
        : m_texPtr{ new_tex, sdl_deleter() }
    {}

    textureAndSize::textureAndSize(Texture newTexP, int newW, int newH)
        : texP{ std::move(newTexP) }, w{ newW }, h{ newH }
    {}

    textureAndSize createTextTexture(Font& font,
        const std::string& text, const Colour& text_colour,
        Renderer& rend)
    {
#ifdef _DEBUG
        errorLogger << "creating a text texture\n";
#endif
        Surface textSurface{
            TTF_RenderUTF8_Blended(font.get(), text.c_str(), text_colour)
        };

        if (!textSurface.get())
            throw SdlException(
                std::string{ "Cannot create surface! SDL_Error: ",
                SDL_GetError() });

        auto tp = SDL_CreateTextureFromSurface(rend.get(), textSurface.get());

        if (!tp) errorLogger << std::string{ SDL_GetError() } << '\n';

        Texture textTexture{
            SDL_CreateTextureFromSurface(rend.get(), textSurface.get())
        };

        if (!textTexture.get()) {
            errorLogger << SDL_GetError() << '\n';
            throw SdlException("Could not create texture!");
        }

        int w{ textSurface.get()->w };
        int h{ textSurface.get()->h };

        return textureAndSize{ std::move(textTexture), w, h };
    }

    // Font creation

    Font::Font(TTF_Font* new_font)
        : m_fontPtr{ new_font, sdl_deleter() }
    {}

    Font createFont(const std::string& path, int font_size)
    {
#ifdef _DEBUG
        errorLogger << "creating a font\n";
#endif
        Font font{ TTF_OpenFont(path.c_str(), font_size) };

        if (!font.get())
            throw SdlException(std::string{
                "Failed to make font! TTF_Error: ",
                TTF_GetError() });

        return font;
    }

    Rect::Rect(SDL_Rect* new_rect)
        : m_rectPtr{ new_rect }
    {}

    Rect::Rect(int x, int y, int w, int h)
        : m_rectPtr{ std::make_unique<SDL_Rect>()}
    {
        m_rectPtr->x = x;
        m_rectPtr->y = y;
        m_rectPtr->w = w;
        m_rectPtr->h = h;
    }

    void process_input(GameWorld& gw, uint32_t windowID,
        std::array<bool, KeyFlag::K_TOTAL>& key_state)
    {
        SDL_Event ev{ };
        key_state[KeyFlag::WINDOW_CHANGE] = false;

        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT)
                key_state[KeyFlag::QUIT] = true;

            else if (ev.type == SDL_WINDOWEVENT) {
                if (ev.window.windowID == windowID)
                    if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        gw.screen.w = ev.window.data1;
                        gw.screen.h = ev.window.data2;
                        key_state[KeyFlag::WINDOW_CHANGE] = true;
                    }
            }

            else if (ev.type == SDL_KEYDOWN) {
                switch (ev.key.keysym.sym) {
                case SDLK_ESCAPE:
                    key_state[KeyFlag::K_ESCAPE] = true;
                    break;
                case SDLK_LEFT: case SDLK_a:
                    key_state[KeyFlag::K_LEFT] = true;
                    break;
                case SDLK_RIGHT: case SDLK_d:
                    key_state[KeyFlag::K_RIGHT] = true;
                    break;
                case SDLK_UP: case SDLK_w:
                    key_state[KeyFlag::K_UP] = true;
                    break;
                case SDLK_DOWN: case SDLK_s:
                    key_state[KeyFlag::K_DOWN] = true;
                    break;
                case SDLK_SPACE: case SDLK_p:
                    key_state[KeyFlag::K_SPACE] = true;
                    break;
                case SDLK_RETURN:
                    key_state[KeyFlag::K_ENTER] = true;
                    break;
                case SDLK_LSHIFT: case SDLK_t:
                    key_state[KeyFlag::K_LSHIFT] = true;
                    break;
                default:
                    break;
                }
            }
            else if (ev.type == SDL_KEYUP) {
                switch (ev.key.keysym.sym) {
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
} // namespace utl