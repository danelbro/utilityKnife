#pragma once

#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <string>

namespace utl {

struct Box;

class TextObject : public Entity {
public:
    TextObject(Box& screen, utl::Renderer& rend, utl::Font& font)
        : Entity{"TEXT", screen, {}}, text{}, m_texture{nullptr}, m_font{font},
          m_size{}, m_rend{rend}, m_col{}
    {}

    TextObject(Box& screen, const Vec2d& pos, utl::Font& font,
               const utl::Colour& color, utl::Renderer& rend);
    TextObject(const TextObject&) = delete;
    TextObject& operator=(const TextObject&) = delete;
    TextObject(TextObject&&) = default;
    TextObject& operator=(TextObject&&) = delete;
    ~TextObject() = default;

    const Vec2d size() { return m_size; }

    void loadFromRenderedText(const std::string& textureText,
                              const utl::Colour& text_colour);
    void recentre();
    void free();
    void render(utl::Renderer& renderer) override;
    void update(double, double) override {}
    void updateText(std::string new_text);
    void setPos(Vec2d new_pos) { m_pos = new_pos; }

private:
    std::string text;
    utl::Texture m_texture;
    utl::Font& m_font;
    Vec2d m_size;
    utl::Renderer& m_rend;
    Colour m_col;
};

}  // namespace utl
