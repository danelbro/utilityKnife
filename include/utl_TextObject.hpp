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
    TextObject() = default;
    TextObject(Stage* stage, Font* font, const Colour& colour,
               const std::string& newText);
    TextObject(Stage* stage, Font* font, const Colour& colour,
               const Vec2d& pos);
    TextObject(Stage* stage, Font* font, const Colour& colour,
               const std::string& newText, const Vec2d& pos);

    void update(double, double) override;
    void render(Renderer& renderer) override;
    const std::string& type() const override;
    const Vec2d& pos() const override;
    const Size& size() const override;
    Stage& stage() override;
    void set_pos(const Vec2d& new_pos) override;

    void set_x_pos(double newX);
    void set_y_pos(double newY);
    void move_x_pos(double shiftX);
    void move_y_pos(double shiftY);

    void recentre(const Box& screen);
    void recentre(const Entity& entity);
    void recentreX(const Box& screen);
    void recentreX(const Entity& entity);
    void recentreY(const Box& screen);
    void recentreY(const Entity& entity);

    void updateText(const std::string& new_text);

public:
    Colour colour{0, 0, 0, 0};

private:
    void free();
    void loadTexture();

private:
    std::string m_type{"TEXT"};
    Stage* m_stage{nullptr};
    Font* m_font{nullptr};
    std::string text{};
    Vec2d m_pos{0.0, 0.0};
    Size m_size{0.0, 0.0};
    Texture m_texture{nullptr};
};

}  // namespace utl
