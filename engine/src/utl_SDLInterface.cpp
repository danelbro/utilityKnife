#include "utl_SDLInterface.hpp"
#include "SDL3/SDL_error.h"
#include "utl_Box.hpp"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

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

void sdl_deleter::operator()(MIX_Mixer* m) const
{
    LOG("destroying a mixer\n");
    MIX_DestroyMixer(m);
}

void sdl_deleter::operator()(MIX_Track* t) const
{
    LOG("destorying a track\n");
    MIX_DestroyTrack(t);
}

void sdl_deleter::operator()(MIX_Audio* a) const
{
    LOG("destroying audio\n");
    MIX_DestroyAudio(a);
}

bool init_SDL(const std::string& title, const std::string& version,
              const std::string& identifier, uint32_t sdlFlags)
{
    if (!SDL_SetAppMetadata(title.c_str(), version.c_str(),
                            identifier.c_str())) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot set SDL App metadata!"};
    }

    if (!SDL_Init(sdlFlags)) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot initialise SDL!"};
    }

    if (!TTF_Init()) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot initialise SDL_TTF!"};
    }

    if (!MIX_Init()) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot initialise SDL_mixer!"};
    }

    LOG("Initialised SDL\n");

    return true;
}

void quit_sdl()
{
    MIX_Quit();
    TTF_Quit();
    SDL_Quit();
    LOG("Quit SDL\n");
}

Window::Window(SDL_Window* new_win) : m_winPtr{new_win, sdl_deleter()} {}

Window::Window(const Window& other) : m_winPtr{nullptr, sdl_deleter()}
{
    auto title{SDL_GetWindowTitle(other.get())};
    int w{};
    int h{};
    SDL_GetWindowSize(other.get(), &w, &h);
    auto flags{SDL_GetWindowFlags(other.get())};

    SDL_Window* window{SDL_CreateWindow(title, w, h, flags)};

    if (!window) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot copy window!"};
    }

    m_winPtr.reset(window);
}

Window& Window::operator=(const Window& other)
{
    Window temp{other};
    m_winPtr.swap(temp.m_winPtr);

    return *this;
}

uint32_t Window::ID() const
{
    return SDL_GetWindowID(m_winPtr.get());
}

SDL_Window* Window::get() const
{
    return m_winPtr.get();
}

Window createWindow(const std::string& title, int w, int h, uint32_t flags)
{
    LOG("creating a window\n");

    SDL_Window* sdlWindow{SDL_CreateWindow(title.c_str(), w, h, flags)};

    if (!sdlWindow) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot create window!"};
    }
    return Window{sdlWindow};
}

Renderer::Renderer(SDL_Renderer* new_rend) : m_rendPtr{new_rend, sdl_deleter()}
{}

Renderer::Renderer(const Renderer& other) : m_rendPtr{nullptr, sdl_deleter()}
{
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
    window = SDL_GetRenderWindow(other.get());
    const char* index{SDL_GetRendererName(other.get())};

    renderer = SDL_CreateRenderer(window, index);

    int vsync{};
    SDL_GetRenderVSync(other.get(), &vsync);
    SDL_SetRenderVSync(renderer, vsync);

    unsigned drawBlendMode{};
    SDL_GetRenderDrawBlendMode(other.get(), &drawBlendMode);
    SDL_SetRenderDrawBlendMode(renderer, drawBlendMode);

    m_rendPtr.reset(renderer);
}

Renderer& Renderer::operator=(const Renderer& other)
{
    Renderer temp{other};
    m_rendPtr.swap(temp.m_rendPtr);

    return *this;
}

SDL_Renderer* Renderer::get() const
{
    return m_rendPtr.get();
}

WindowWithRenderer::WindowWithRenderer(Window&& w, Renderer&& r)
    : window{std::move(w)}, renderer{std::move(r)}
{}

WindowWithRenderer create_window_with_renderer(const std::string& title, int w,
                                               int h, uint32_t flags)
{
    LOG("Creating a window + renderer\n");

    SDL_Window* sdlWindow{nullptr};
    SDL_Renderer* sdlRenderer{nullptr};

    if (!SDL_CreateWindowAndRenderer(title.c_str(), w, h, flags, &sdlWindow,
                                     &sdlRenderer)) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot create window/renderer!"};
    }

    return {Window{sdlWindow}, Renderer{sdlRenderer}};
}

Renderer createRenderer(const Window& window, const char* index)
{
    LOG("creating a renderer\n");

    SDL_Renderer* sdlRenderer{SDL_CreateRenderer(window.get(), index)};

    if (!sdlRenderer) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot create renderer!"};
    }
    return Renderer{sdlRenderer};
}

bool Renderer::setVSync(int vsync)
{
    bool isVSyncSet{SDL_SetRenderVSync(m_rendPtr.get(), vsync)};
    if (!isVSyncSet) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Couldn't set vsync!"};
    }
    return isVSyncSet;
}

bool Renderer::setDrawingBlendMode(unsigned blendMode)
{
    bool isBlendModeSet{SDL_SetRenderDrawBlendMode(get(), blendMode)};

    if (!isBlendModeSet) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Couldn't set blend mode!"};
    }
    return isBlendModeSet;
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

void drawPoint(Renderer& rend, double x, double y)
{
    SDL_RenderPoint(rend.get(), static_cast<float>(x), static_cast<float>(y));
}

Surface::Surface(SDL_Surface* new_surf) : m_surfPtr{new_surf, sdl_deleter()} {}

Surface::Surface(const Surface& other) : m_surfPtr{nullptr, sdl_deleter()}
{
    SDL_Surface* surf{nullptr};
    surf = SDL_CreateSurfaceFrom(other.get()->w, other.get()->h,
                                 other.get()->format, other.get()->pixels,
                                 other.get()->pitch);
    if (!surf) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot copy surface!"};
    }

    m_surfPtr.reset(surf);
}

Surface& Surface::operator=(const Surface& other)
{
    Surface temp{other};
    m_surfPtr.swap(temp.m_surfPtr);

    return *this;
}

SDL_Surface* Surface::get() const
{
    return m_surfPtr.get();
}

Texture::Texture(SDL_Texture* new_tex) : m_texPtr{new_tex, sdl_deleter()} {}

Texture::Texture(const Texture& other) : m_texPtr{nullptr, sdl_deleter()}
{
    SDL_Renderer* renderer{nullptr};
    renderer = SDL_GetRendererFromTexture(other.get());
    if (!renderer) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to get renderer while copying texture!"};
    }

    SDL_Window* window{nullptr};
    window = SDL_GetRenderWindow(renderer);
    if (!window) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to get window while copying texture!"};
    }

    SDL_PixelFormat format{SDL_GetWindowPixelFormat(window)};
    if (!format) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{
            "Failed to get window pixel format while copying texture!"};
    }

    auto accessnum{SDL_GetNumberProperty(SDL_GetTextureProperties(other.get()),
                                         "SDL_PROP_TEXTURE_ACCESS_NUMBER",
                                         SDL_TEXTUREACCESS_STATIC)};
    float w{};
    float h{};
    if (!SDL_GetTextureSize(other.get(), &w, &h)) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to get texture size while copying texture!"};
    }

    SDL_Texture* tex{nullptr};
    tex = SDL_CreateTexture(renderer, format, SDL_TextureAccess(accessnum),
                            static_cast<int>(w), static_cast<int>(h));

    if (!tex) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot copy texture!"};
    }

    m_texPtr.reset(tex);
}

Texture& Texture::operator=(const Texture& other)
{
    Texture temp{other};
    m_texPtr.swap(temp.m_texPtr);

    return *this;
}

SDL_Texture* Texture::get() const
{
    return m_texPtr.get();
}

void Texture::reset(SDL_Texture* new_ptr)
{
    m_texPtr.reset(new_ptr);
}

textureAndSize::textureAndSize(Texture&& newTexP, int newW, int newH)
    : texP{std::move(newTexP)}, w{newW}, h{newH}
{}

textureAndSize createTextTexture(const Font& font, const std::string& text,
                                 const Colour& text_colour, Renderer& rend)
{
    LOG("creating a surface\n");
    SDL_Surface* textSurface{TTF_RenderText_Blended(
        font.get(), text.c_str(), text.length(), text_colour)};

    if (!textSurface) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Cannot create surface!"};
    }

    int w{textSurface->w};
    int h{textSurface->h};

    LOG("creating a texture from the surface\n");
    SDL_Texture* textTexture{
        SDL_CreateTextureFromSurface(rend.get(), textSurface)};

    if (!textTexture) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Could not create texture!"};
    }

    LOG("destroying the surface\n");
    SDL_DestroySurface(textSurface);

    return textureAndSize{textTexture, w, h};
}

Font::Font(TTF_Font* new_font, const std::filesystem::path& path)
    : m_fontPtr{new_font, sdl_deleter()}, m_path{path}
{}

Font::Font(const Font& other) : m_fontPtr{nullptr, sdl_deleter()}

{
    TTF_Font* font{nullptr};
    font = TTF_OpenFont(other.m_path.string().c_str(),
                        TTF_GetFontSize(other.get()));
    if (!font) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to copy Font!"};
    }

    m_fontPtr.reset(font);
}

Font& Font::operator=(const Font& other)
{
    Font temp{other};
    m_fontPtr.swap(temp.m_fontPtr);
    std::swap(this->m_path, temp.m_path);
    return *this;
}

TTF_Font* Font::get() const
{
    return m_fontPtr.get();
}

Font createFont(const std::filesystem::path& path, int font_size)
{
    LOG("creating a font\n");

    TTF_Font* font{
        TTF_OpenFont(path.string().c_str(), static_cast<float>(font_size))};

    if (!font) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to make font!"};
    }
    return Font{font, path};
}

Rect::Rect() : m_rectPtr{nullptr} {}

Rect::Rect(SDL_FRect* new_rect) : m_rectPtr{new_rect} {}

Rect::Rect(const RectDimensions& rect)
    : m_rectPtr{std::make_unique<SDL_FRect>(
          static_cast<float>(rect.x), static_cast<float>(rect.y),
          static_cast<float>(rect.w), static_cast<float>(rect.h))}
{}

Rect::Rect(const Rect& other)
    : m_rectPtr{std::make_unique<SDL_FRect>(other.get()->x, other.get()->y,
                                            other.get()->w, other.get()->y)}

{}

Rect& Rect::operator=(const Rect& other)
{
    Rect temp{other};
    m_rectPtr.swap(temp.m_rectPtr);
    return *this;
}

SDL_FRect* Rect::get() const
{
    return m_rectPtr.get();
}

const float& Rect::x() const
{
    return m_rectPtr.get()->x;
}

const float& Rect::y() const
{
    return m_rectPtr.get()->y;
}

const float& Rect::w() const
{
    return m_rectPtr.get()->w;
}

const float& Rect::h() const
{
    return m_rectPtr.get()->h;
}

void Rect::reset(const RectDimensions& rect)
{
    if (!m_rectPtr)
        m_rectPtr = std::make_unique<SDL_FRect>(rect.x, rect.y, rect.w, rect.h);

    else {
        m_rectPtr->x = rect.x;
        m_rectPtr->y = rect.y;
        m_rectPtr->w = rect.w;
        m_rectPtr->h = rect.h;
    }
}

void Rect::draw(Renderer& renderer)
{
    SDL_RenderFillRect(renderer.get(), m_rectPtr.get());
}

Mixer::Mixer(std::uint32_t playback_dev_id, const SDL_AudioSpec* spec)
    : m_mixerPtr{MIX_CreateMixerDevice(playback_dev_id, spec), sdl_deleter()}
{
    if (!m_mixerPtr) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to make mixer!"};
    }
}

MIX_Mixer* Mixer::get()
{
    return m_mixerPtr.get();
}

Track::Track(Mixer& mixer)
    : m_trackPtr{MIX_CreateTrack(mixer.get()), sdl_deleter()}
{
    if (!m_trackPtr) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to make track!"};
    }
}

Music::Music(Mixer& mixer, std::filesystem::path data)
    : m_musicPtr{MIX_LoadAudio(mixer.get(), data.string().c_str(), true)}
{
    if (!m_musicPtr) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to load music!"};
    }
}

Effect::Effect(Mixer& mixer, std::filesystem::path data)
    : m_effectPtr{MIX_LoadAudio(mixer.get(), data.string().c_str(), true)}
{
    if (!m_effectPtr) {
        ERRLOGF("%s\n", SDL_GetError());
        throw SdlException{"Failed to load effect!"};
    }
}

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
            case SDLK_LCTRL:
                key_state[KeyFlag::K_LCTRL] = true;
                break;
            case SDLK_Z:
                key_state[KeyFlag::K_Z] = true;
                break;
            case SDLK_X:
                key_state[KeyFlag::K_X] = true;
                break;
            case SDLK_C:
                key_state[KeyFlag::K_C] = true;
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
            case SDLK_RETURN:
                key_state[KeyFlag::K_ENTER] = false;
                break;
            case SDLK_LSHIFT:
                key_state[KeyFlag::K_LSHIFT] = false;
                break;
            case SDLK_LCTRL:
                key_state[KeyFlag::K_LCTRL] = false;
                break;
            case SDLK_Z:
                key_state[KeyFlag::K_Z] = false;
                break;
            case SDLK_X:
                key_state[KeyFlag::K_X] = false;
                break;
            case SDLK_C:
                key_state[KeyFlag::K_C] = false;
                break;
            default:
                break;
            }
        }
    }
}

}  // namespace utl
