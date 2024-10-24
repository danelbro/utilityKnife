#include "utl_TextObject.hpp"

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <string>

namespace utl {

TextObject::TextObject(Box& screen, const Vec2d& pos, utl::Font& font,
                       const Colour& color, utl::Renderer& rend)
    : Entity{"TEXT", screen, pos}, text{}, m_texture{nullptr}, m_font{font},
      m_size{0, 0}, m_rend{rend}, m_col{color}
{}

void TextObject::free()
{
    m_texture.reset(nullptr);
    m_size = {0, 0};
}

void TextObject::loadFromRenderedText(const std::string& textureText,
                                      const utl::Colour& text_colour)
{
    free();

    auto texPstruct{
        utl::createTextTexture(m_font, textureText, text_colour, m_rend)};

    m_texture.reset(texPstruct.texP.get());
    texPstruct.texP.reset(nullptr);

    m_size.x = texPstruct.w;
    m_size.y = texPstruct.h;
}

void TextObject::recentre()
{
    m_pos.x = m_screenSpace.w / 2.0 - m_size.x / 2.0;
    m_pos.y = m_screenSpace.h / 2.0 - m_size.y / 2.0;
}

void TextObject::updateText(std::string new_text)
{
    text = new_text;
    loadFromRenderedText(text, m_col);
}

void TextObject::render(utl::Renderer& renderer)
{
    utl::Rect renderQuad{static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
                         static_cast<int>(m_size.x),
                         static_cast<int>(m_size.y)};
    utl::Rect nullRect{nullptr};
    utl::copyTexturePortion(renderer, m_texture, nullRect, renderQuad);
}

}  // namespace utl
