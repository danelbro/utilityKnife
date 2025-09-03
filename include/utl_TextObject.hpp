#pragma once

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <string>

namespace utl {

class TextObject : public Entity {
public:
    TextObject(Box& screen, utl::Renderer& rend, utl::Font& font)
        : Entity{"TEXT", screen, {}}, text{}, m_texture{nullptr}, m_font{font},
          m_size{}, m_rend{rend}, m_col{}
    {}
    TextObject(Box& screen, utl::Renderer& rend, utl::Font& font,
               const std::string& newText, const utl::Colour& color);
    TextObject(Box& screen, utl::Renderer& rend, utl::Font& font,
               const Vec2d& pos, const utl::Colour& color);
    TextObject(Box& screen, utl::Renderer& rend, utl::Font& font,
               const std::string& newText, const Vec2d& pos,
               const utl::Colour& color);

    ~TextObject() = default;
    TextObject(const TextObject&) = delete;
    TextObject& operator=(const TextObject&) = delete;
    TextObject(TextObject&&) = default;
    TextObject& operator=(TextObject&&) = delete;

    Vec2d size() const override { return m_size; }

    void loadFromRenderedText(const std::string& textureText,
                              const utl::Colour& text_colour);
    void recentre();
    void recentreToEntityX(const Entity& entity);  // recentres to the central X
                                                   // of the provided Entity
    void recentreToEntityY(const Entity& entity);  // recentres to the central Y
                                                   // of the provided Entity
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
