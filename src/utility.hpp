#pragma once

#include <fstream>
#include <random>

struct Vec2d;

namespace utl {
    extern std::ofstream errorLogger;

    Vec2d randomPos(std::mt19937& rng, int w, int h);

    std::mt19937 makeSeededRNG();

    enum class EntityFlag {
        PLAYER,
        ASTEROID,
        ENEMY,
        BULLET,
        ENEMY_BULLET,
        TEXT,
        E_TYPE_TOTAL
    };

    enum class StageID {
        TITLE_SCREEN,
        PLAYING,
        HIGH_SCORES,
        STAGES_TOTAL,
        QUIT
    };
}
