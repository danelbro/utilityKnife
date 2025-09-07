#pragma once

#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_TextObject.hpp"
#include "utl_Vec2d.hpp"

#include <string>
#include <vector>

namespace utl {

class Stage;

class ScoreBoard : public Entity {
public:
    ScoreBoard(Stage& stage, Font& font, const Vec2d& pos = {0.0, 0.0},
               double padding = 0.0, const Colour& color = {0, 0, 0, 0},
               const Colour& newScoreColor = {0, 0, 0, 0},
               const std::vector<std::string>& scores = {});

    void update(double, double) override {}
    void render(Renderer& renderer) override;

    const std::string& type() const override { return m_type; }
    const Vec2d& pos() const override { return m_pos; }
    const Size& size() const override { return m_size; }
    const Stage& stage() const override { return m_stage; }

    void set_pos(const Vec2d& newPos) override;

    void change_padding(double padding);
    void change_font(Font& font);
    void change_renderer(Renderer& renderer);
    void set_text(const std::vector<std::string>& scores, int newScore = -1);

public:
    Colour textColor;
    Colour newScoreColor;

private:
    void reposition_text();

private:
    const std::string m_type;
    Size m_size;
    Vec2d m_pos;
    double m_padding;
    Font& m_font;
    Stage& m_stage;
    std::vector<TextObject> m_scores;
};

}  // namespace utl
