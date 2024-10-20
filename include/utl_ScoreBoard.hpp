#pragma once

#include "utl_Box.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_TextObject.hpp"
#include "utl_Vec2d.hpp"

#include <string>
#include <vector>

namespace utl {

class ScoreBoard : public Entity {
public:
    ScoreBoard(Box& screen, const Vec2d& pos, double padding, Font& font,
               const Colour& color, Renderer& renderer);
    ScoreBoard(Box& screen, const Vec2d& pos, double padding, Font& font,
               const Colour& color, Renderer& renderer,
               const std::vector<std::string>& scores);
    void update(double, double) override {}
    void render(Renderer& renderer) override;

    void set_text(const std::vector<std::string>&);
    void set_pos(double x, double y);
    void set_pos(const Vec2d& newPos);
    void change_padding(double padding);

    const Vec2d& size() const { return m_size; }

private:
    void reposition_text();

    double m_padding;
    Font& m_font;
    Colour m_col;
    Renderer& m_renderer;
    std::vector<TextObject> m_scores;
    Vec2d m_size;
};

}  // namespace utl
