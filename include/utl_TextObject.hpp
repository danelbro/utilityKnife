#pragma once

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <string>

namespace utl {

class Stage;

class TextObject : public Entity {
public:
    TextObject();
    TextObject(const Font* font, const Stage* stage);
    TextObject(const Font* font, const Stage* stage, const Colour& colour,
               const Vec2d& pos);
    TextObject(const Font* font, const Stage* stage, const Colour& colour,
               const std::string& newText, Renderer& renderer);
    TextObject(const Font* font, const Stage* stage, const Colour& colour,
               const Vec2d& pos, const std::string& newText,
               Renderer& renderer);

    void update(double, double) override {}
    void render(Renderer& renderer) override;
    const std::string& type() const override { return m_type; }
    const Vec2d& pos() const override { return m_pos; }
    const Vec2d& size() const override { return m_size; }
    const Stage& stage() const override { return *m_stage; }
    void set_pos(double x, double y) override { m_pos = {x, y}; }
    void set_pos(const Vec2d& new_pos) override { m_pos = new_pos; }

    void updateText(std::string new_text, Renderer& renderer);
    void loadFromRenderedText(Renderer& renderer,
                              const std::string& textureText,
                              const Colour& text_colour);

    void recentre(const Box& screen);
    void recentre(const Entity& entity);
    void recentreX(const Box& screen);
    void recentreX(const Entity& entity);
    void recentreY(const Box& screen);
    void recentreY(const Entity& entity);

    void free();

public:
    Colour colour;

private:
    const std::string m_type;
    Vec2d m_size;
    Vec2d m_pos;
    const Font* m_font;
    const Stage* m_stage;
    std::string text;
    Texture m_texture;
};

}  // namespace utl
