#include "utl_ScoreBoard.hpp"

#include "utl_SDLInterface.hpp"
#include "utl_Stage.hpp"
#include "utl_TextObject.hpp"
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

namespace utl {

static double calculate_width(const std::vector<TextObject>& scores);
static double calculate_height(const std::vector<TextObject>& scores,
                               double padding);

ScoreBoard::ScoreBoard(Stage& stage, Font& font, const Vec2d& pos,
                       double padding, const Colour& textColor,
                       const Colour& newScoreColor,
                       const std::vector<std::string>& scores)
    : Entity{}, textColor{textColor}, newScoreColor{newScoreColor},
      m_type{"SCOREBOARD"}, m_size{}, m_pos{pos}, m_padding{padding},
      m_font{font}, m_stage{stage}, m_scores{}
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

void ScoreBoard::set_text(const std::vector<std::string>& scores,
                          int newScorePos)
{
    for (size_t i{0}; i < scores.size(); i++) {
        if (newScorePos == static_cast<int>(i)) {
            m_scores.emplace_back(m_stage, m_font, newScoreColor, m_pos,
                                  scores[i]);
        } else {
            m_scores.emplace_back(m_stage, m_font, textColor, m_pos, scores[i]);
        }
    }
    m_size = {calculate_width(m_scores), calculate_height(m_scores, m_padding)};
    reposition_text();
}

void ScoreBoard::set_pos(const Vec2d& pos)
{
    m_pos = pos;
    reposition_text();
}

void ScoreBoard::change_padding(double padding)
{
    m_padding = padding;
    reposition_text();
}

void ScoreBoard::reposition_text()
{
    double running_y_pos{m_pos.y};

    for (auto& score : m_scores) {
        score.recentreX(*this);
        score.set_pos({score.pos().x, running_y_pos});
        running_y_pos += score.size().h + m_padding;
    }
}

static double calculate_width(const std::vector<TextObject>& scores)
{
    auto widest = std::ranges::max_element(
        scores, [](const auto& score1, const auto& score2) {
            return score1.size().w > score2.size().w;
        });
    return widest->size().w;
}

static double calculate_height(const std::vector<TextObject>& scores,
                               double padding)
{
    auto height = std::accumulate(scores.begin(), scores.end(), 0.0,
                                  [](double acc, const auto& score) {
                                      return std::move(acc) + score.size().h;
                                  });
    height += padding * (scores.size() - 1);

    return height;
}

}  // namespace utl
