#include "utl_TextObject.hpp"

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Stage.hpp"
#include "utl_Vec2d.hpp"

#include <string>

namespace utl {

TextObject::TextObject(Font& font, Stage& stage, const Colour& color,
                       const Vec2d& pos, const std::string& newText)
    : Entity{}, colour{color}, m_type{"TEXT"}, m_size{0, 0}, m_pos{pos},
      m_stage{stage}, m_font{font}, text{newText}, m_texture{nullptr}
{
    loadTexture();
}

void TextObject::free()
{
    m_texture.reset(nullptr);
    m_size = {0, 0};
}

void TextObject::updateText(const std::string& new_text)
{
    text = new_text;
    loadTexture();
}

void TextObject::loadTexture()
{
    free();
    auto texPstruct{
        createTextTexture(m_font, text, colour, m_stage.renderer())};
    m_texture = std::move(texPstruct.texP);
    m_size = {texPstruct.w, texPstruct.h};
}

static void recentreX_(TextObject& to, double least, double width)
{
    to.set_pos({least + width / 2.0 - to.size().h / 2.0, to.pos().y});
}

static void recentreY_(TextObject& to, double least, double height)
{
    to.set_pos({to.pos().x, least + height / 2.0 - to.size().h / 2.0});
}

void TextObject::recentre(const Box& screen)
{
    recentreX_(*this, 0.0, screen.w);
    recentreY_(*this, 0.0, screen.h);
}

void TextObject::recentre(const Entity& entity)
{
    recentreX_(*this, entity.pos().x, entity.size().w);
    recentreY_(*this, entity.pos().y, entity.size().h);
}

void TextObject::recentreX(const Box& screen)
{
    recentreX_(*this, 0.0, screen.w);
}

void TextObject::recentreX(const Entity& entity)
{
    recentreX_(*this, entity.pos().x, entity.size().w);
}

void TextObject::recentreY(const Box& screen)
{
    recentreY_(*this, 0.0, screen.h);
}

void TextObject::recentreY(const Entity& entity)
{
    recentreY_(*this, entity.pos().y, entity.size().h);
}

void TextObject::render(Renderer& renderer)
{
    Rect renderQuad{{static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
                     m_size.w, m_size.h}};
    Rect nullRect{nullptr};
    copyTexturePortion(renderer, m_texture, nullRect, renderQuad);
}

}  // namespace utl
