#include "utl_ScoreBoard.hpp"

#include "utl_SDLInterface.hpp"
#include "utl_TextObject.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace utl {

static double calculate_width(const std::vector<TextObject>& scores);
static double calculate_height(const std::vector<TextObject>& scores,
                               double padding);

ScoreBoard::ScoreBoard(Box& screen, const Vec2d& pos, double padding,
                       Font& font, const Colour& color, Renderer& renderer)
    : Entity{"SCOREBOARD", screen, pos}, m_padding{padding}, m_font{font},
      m_col{color}, m_renderer{renderer}, m_scores{}, m_size{}
{}

ScoreBoard::ScoreBoard(Box& screen, const Vec2d& pos, double padding,
                       Font& font, const Colour& color, Renderer& renderer,
                       const std::vector<std::string>& scores)
    : Entity{"SCOREBOARD", screen, pos}, m_padding{padding}, m_font{font},
      m_col{color}, m_renderer{renderer}, m_scores{}, m_size{}
{
    m_scores.reserve(5);
    set_text(scores);
    set_pos(m_pos);
}

void ScoreBoard::render(Renderer& renderer)
{
    for (auto& score : m_scores) {
        score.render(renderer);
    }
}

void ScoreBoard::set_text(const std::vector<std::string>& scores)
{
    for (const auto& score : scores) {
        m_scores.emplace_back(m_screenSpace, m_pos, m_font, m_col, m_renderer);
        m_scores.back().updateText(score);
    }
    m_size.x = calculate_width(m_scores);
    m_size.y = calculate_height(m_scores, m_padding);
    reposition_text();
}

void ScoreBoard::set_pos(const Vec2d& pos)
{
    m_pos = pos;
    reposition_text();
}

void ScoreBoard::set_pos(double x, double y)
{
    set_pos({x, y});
}

void ScoreBoard::reposition_text()
{
    double runningHeight{m_pos.y};
    for (size_t i{0}; i < m_scores.size(); i++) {
        double x{}, y{};
        x = m_pos.x + (m_size.x / 2) - (m_scores[i].size().x / 2);
        y = runningHeight;
        runningHeight += m_scores[i].size().y + m_padding;
        m_scores[i].setPos({x, y});
    }
}

static double calculate_width(const std::vector<utl::TextObject>& scores)
{
    double width{0.0};
    for (const auto& score : scores) {
        if (score.size().x > width) {
            width = score.size().x;
        }
    }
    return width;
}

static double calculate_height(const std::vector<utl::TextObject>& scores,
                               double padding)
{
    if (scores.empty()) {
        return 0.0;
    }

    double height{0.0};
    for (const auto& score : scores) {
        height += score.size().y;
    }
    height += padding * (scores.size() - 1);

    return height;
}

}  // namespace utl
