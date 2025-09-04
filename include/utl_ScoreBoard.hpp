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
    ScoreBoard();
    ScoreBoard(const Vec2d& pos, double padding, Font& font,
               const Colour& mainTextColor, const Colour& newScoreColor,
               Renderer& renderer, Stage& stage);
    ScoreBoard(const Vec2d& pos, double padding, Font& font,
               const Colour& color, const Colour& newScoreColor,
               Renderer& renderer, Stage& stage,
               const std::vector<std::string>& scores);

    void update(double, double) override {}
    void render(Renderer& renderer) override;

    const std::string& type() const override { return m_type; }
    const Vec2d& pos() const override { return m_pos; }
    const Vec2d& size() const override { return m_size; }
    const Stage& stage() const override { return *m_stage; }

    void set_pos(double x, double y) override;
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
    Vec2d m_size;
    Vec2d m_pos;
    double m_padding;
    const Font* m_font;
    Renderer* m_renderer;
    const Stage* m_stage;
    std::vector<TextObject> m_scores;
};

}  // namespace utl
