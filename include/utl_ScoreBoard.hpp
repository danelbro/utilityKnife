#pragma once

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Vec2d.hpp"

#include <string>
#include <vector>

namespace utl {

class ScoreBoard : public Entity {
public:
    ScoreBoard(Box& screen, const Vec2d& pos, Font& font, const Colour& color,
               Renderer& renderer)
        : Entity{"SCOREBOARD", screen, pos}, m_font{font}, m_col{color},
          m_renderer{renderer}
    {}

    void update(double, double) override {}
    void render(Renderer&) override {}

    void set_text(std::vector<std::string>) {}
    void set_pos(double, double) {}

private:
    Font& m_font;
    Colour m_col;
    Renderer& m_renderer;
};

}  // namespace utl
