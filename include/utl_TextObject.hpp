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
    TextObject(Stage& stage, Font& font, const Colour& colour = {0, 0, 0, 0},
               const Vec2d& pos = {0.0, 0.0}, const std::string& newText = {});

    void update(double, double) override {}
    void render(Renderer& renderer) override;
    const std::string& type() const override { return m_type; }
    const Vec2d& pos() const override { return m_pos; }
    const Size& size() const override { return m_size; }
    const Stage& stage() const override;
    void set_pos(const Vec2d& new_pos) override { m_pos = new_pos; }

    void updateText(const std::string& new_text);

    void recentre(const Box& screen);
    void recentre(const Entity& entity);
    void recentreX(const Box& screen);
    void recentreX(const Entity& entity);
    void recentreY(const Box& screen);
    void recentreY(const Entity& entity);

public:
    Colour colour;

private:
    void free();
    void loadTexture();

private:
    const std::string m_type;
    Size m_size;
    Vec2d m_pos;
    Stage& m_stage;
    Font& m_font;
    std::string text;
    Texture m_texture;
};

}  // namespace utl
