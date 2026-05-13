#include "utl_TextObject.hpp"

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Stage.hpp"
#include "utl_Vec2d.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace utl {

static void recentreX_(TextObject& to, double least, double width);
static void recentreY_(TextObject& to, double least, double height);

TextObject::TextObject(Stage* stage, Font* font, const Colour& colour,
                       const std::string& newText)
    : TextObject{stage, font, colour, newText, {0.0, 0.0}}
{}

TextObject::TextObject(Stage* stage, Font* font, const Colour& colour,
                       const Vec2d& pos)
    : TextObject{stage, font, colour, {}, pos}
{}

TextObject::TextObject(Stage* stage, Font* font, const Colour& color,
                       const std::string& newText, const Vec2d& pos)
    : Entity{}, colour{color}, m_stage{stage}, m_font{font}, text{newText},
      m_pos{pos}, m_texture{nullptr}
{
    if (!m_stage || !m_font)
        throw std::runtime_error("creating stateful utl::TextObject without a "
                                 "font or an owning utl::Stage!");
    loadTexture();
}

void TextObject::update(std::chrono::milliseconds, std::chrono::milliseconds) {}

void TextObject::render(Renderer& renderer)
{
    Rect renderQuad{{static_cast<float>(m_pos.x), static_cast<float>(m_pos.y),
                     static_cast<float>(m_size.w),
                     static_cast<float>(m_size.h)}};
    Rect nullRect{nullptr};
    copyTexturePortion(renderer, m_texture, nullRect, renderQuad);
}

const std::string& TextObject::type() const
{
    return m_type;
}

const Vec2d& TextObject::pos() const
{
    return m_pos;
}

const Size& TextObject::size() const
{
    return m_size;
}

Stage& TextObject::stage()
{
    if (!m_stage)
        throw std::runtime_error("TextObject has no owner!");
    return *m_stage;
}

void TextObject::set_pos(const Vec2d& new_pos)
{
    m_pos.x = std::trunc(new_pos.x);
    m_pos.y = std::trunc(new_pos.y);
}

void TextObject::set_x_pos(double newX)
{
    m_pos.x = std::trunc(newX);
}

void TextObject::set_y_pos(double newY)
{
    m_pos.y = std::trunc(newY);
}

void TextObject::move_x_pos(double shiftX)
{
    m_pos.x += shiftX;
    m_pos.x = std::trunc(m_pos.x);
}

void TextObject::move_y_pos(double shiftY)
{
    m_pos.y += shiftY;
    m_pos.y = std::trunc(m_pos.y);
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

void TextObject::updateText(const std::string& new_text)
{
    text = new_text;
    loadTexture();
}

void TextObject::free()
{
    m_texture.reset(nullptr);
    m_size = {0, 0};
}

void TextObject::loadTexture()
{
    if (!m_stage || !m_font) {
        LOG("Trying to load texture on default-initialized TextObject");
        return;
    }
    free();
    auto texPstruct{
        createTextTexture(*m_font, text, colour, m_stage->renderer())};
    m_size = {static_cast<double>(texPstruct.w),
              static_cast<double>(texPstruct.h)};
    m_texture = std::move(texPstruct.texP);
}

static void recentreX_(TextObject& to, double least, double width)
{
    to.set_pos({least + width / 2.0 - to.size().w / 2.0, to.pos().y});
}

static void recentreY_(TextObject& to, double least, double height)
{
    to.set_pos({to.pos().x, least + height / 2.0 - to.size().h / 2.0});
}

}  // namespace utl
