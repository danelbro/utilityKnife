#include "TextObject.hpp"

#include <string>
#include <vector>

#include "SDL_Interface.hpp"
#include "Entity.hpp"
#include "GameWorld.hpp"
#include "utility.hpp"
#include "Vec2d.hpp"

TextObject::TextObject(GameWorld& gw, const Vec2d& pos, utl::Font& font,
    const utl::Colour& color, utl::Renderer& rend)
    : Entity{ "TEXT", gw, pos,
    std::vector<Vec2d>{}, color, 1.0 },
      text{ }, m_texture{ nullptr }, m_font{ font }, m_size{ 0, 0 },
      m_rend{ rend }
{}

void TextObject::free()
{
    m_texture.reset(nullptr);
    m_size = { 0, 0 };
}

void TextObject::loadFromRenderedText(const std::string& textureText,
                                      const utl::Colour& text_colour)
{
    free();

    auto texPstruct{ utl::createTextTexture(m_font, textureText,
        text_colour, m_rend) };

    m_texture = std::move(texPstruct.texP);
    m_size.x = texPstruct.w;
    m_size.y = texPstruct.h;
}

void TextObject::recentre()
{
    m_pos.x = gameWorld.screen.w / 2.0 - m_size.x / 2.0;
    m_pos.y = gameWorld.screen.h / 2.0 - m_size.y / 2.0;
}

void TextObject::updateText(std::string new_text)
{
    text = new_text;
    loadFromRenderedText(text, m_color);
}

void TextObject::render(utl::Renderer& renderer)
{
    utl::Rect renderQuad{ static_cast<int>(m_pos.x),
                          static_cast<int>(m_pos.y),
                          static_cast<int>(m_size.x),
                          static_cast<int>(m_size.y)
    };
    utl::Rect nullRect{ nullptr };
    utl::copyTexturePortion(renderer, m_texture, nullRect, renderQuad);
}
