#pragma once

#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_TextObject.hpp"
#include "utl_Vec2d.hpp"

#include <chrono>
#include <string>
#include <vector>

namespace utl {

class Stage;

class ScoreBoard : public Entity {
public:
    ScoreBoard() = default;
    ScoreBoard(Stage* stage, Font* font, const Vec2d& pos, double padding,
               const Colour& color, const Colour& newScoreColor,
               const std::vector<std::string>& scores);

    void update(std::chrono::milliseconds, std::chrono::milliseconds) override;
    void render(Renderer& renderer) override;
    const std::string& type() const override;
    const Vec2d& pos() const override;
    const Size& size() const override;
    Stage& stage() override;
    void set_pos(const Vec2d& newPos) override;

    void change_padding(double padding);
    void set_text(const std::vector<std::string>& scores, int newScore = -1);

public:
    Colour textColor{0, 0, 0, 0};
    Colour newScoreColor{0, 0, 0, 0};

private:
    void reposition_text();

private:
    const std::string m_type{"SCOREBOARD"};
    Size m_size{0.0, 0.0};
    Vec2d m_pos{0.0, 0.0};
    double m_padding{0.0};
    Font* m_font{nullptr};
    Stage* m_stage{nullptr};
    std::vector<TextObject> m_scores{};
};

}  // namespace utl
