#include "utl_TextObject.hpp"

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Stage.hpp"
#include "utl_Vec2d.hpp"

#include <string>

namespace utl {

TextObject::TextObject()
    : Entity{}, colour{}, m_type{"TEXT"}, m_size{}, m_pos{}, m_font{nullptr},
      m_stage{nullptr}, text{}, m_texture{nullptr}
{}

TextObject::TextObject(const Font* font, const Stage* stage)
    : Entity{}, colour{}, m_type{"TEXT"}, m_size{}, m_pos{}, m_font{font},
      m_stage{stage}, text{}, m_texture{nullptr}
{}

TextObject::TextObject(const Font* font, const Stage* stage, const Colour& col,
                       const Vec2d& pos)
    : Entity{}, colour{col}, m_type{"TEXT"}, m_size{0, 0}, m_pos{pos},
      m_font{font}, m_stage{stage}, text{}, m_texture{nullptr}
{}

TextObject::TextObject(const Font* font, const Stage* stage, const Colour& col,
                       const std::string& newText, Renderer& renderer)
    : Entity{}, colour{col}, m_type{"TEXT"}, m_size{0, 0}, m_pos{},
      m_font{font}, m_stage{stage}, text{newText}, m_texture{nullptr}
{
    loadFromRenderedText(renderer, text, colour);
}

TextObject::TextObject(const Font* font, const Stage* stage,
                       const Colour& color, const Vec2d& pos,
                       const std::string& newText, Renderer& renderer)
    : Entity{}, colour{color}, m_type{"TEXT"}, m_size{0, 0}, m_pos{pos},
      m_font{font}, m_stage{stage}, text{newText}, m_texture{nullptr}
{
    loadFromRenderedText(renderer, text, colour);
}

void TextObject::free()
{
    m_texture.reset(nullptr);
    m_size = {0, 0};
}

void TextObject::loadFromRenderedText(Renderer& renderer,
                                      const std::string& textureText,
                                      const Colour& text_colour)
{
    free();

    auto texPstruct{
        createTextTexture(*m_font, textureText, text_colour, renderer)};

    m_texture = std::move(texPstruct.texP);

    m_size.x = texPstruct.w;
    m_size.y = texPstruct.h;
}

static void recentreX_(TextObject& to, double least, double width)
{
    to.set_pos({least + width / 2.0 - to.size().x / 2.0, to.pos().y});
}

static void recentreY_(TextObject& to, double least, double height)
{
    to.set_pos({to.pos().x, least + height / 2.0 - to.size().y / 2.0});
}

void TextObject::recentre(const Box& screen)
{
    recentreX_(*this, 0.0, screen.w);
    recentreY_(*this, 0.0, screen.h);
}

void TextObject::recentre(const Entity& entity)
{
    recentreX_(*this, entity.pos().x, entity.size().x);
}

void TextObject::recentreX(const Box& screen)
{
    recentreX_(*this, 0.0, screen.w);
}

void TextObject::recentreX(const Entity& entity)
{
    recentreX_(*this, entity.pos().x, entity.size().x);
}

void TextObject::recentreY(const Box& screen)
{
    recentreY_(*this, 0.0, screen.h);
}

void TextObject::recentreY(const Entity& entity)
{
    recentreY_(*this, entity.pos().y, entity.size().y);
}

void TextObject::updateText(std::string new_text, Renderer& renderer)
{
    text = new_text;
    loadFromRenderedText(renderer, text, colour);
}

void TextObject::render(Renderer& renderer)
{
    Rect renderQuad{static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
                    static_cast<int>(m_size.x), static_cast<int>(m_size.y)};
    Rect nullRect{nullptr};
    copyTexturePortion(renderer, m_texture, nullRect, renderQuad);
}

}  // namespace utl
