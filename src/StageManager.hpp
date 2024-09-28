#pragma once

#include <array>
#include <unordered_map>
#include <memory>

#include <SDL.h>

#include "Stage.hpp"
#include "utility.hpp"

class Application;

class StageManager {
public:
    StageManager(Application& app);

    const utl::StageID& get_current() const { return current; }
    Stage* get_current_stage() { return stages[current].get(); }
    Stage* get_next_stage() { return stages[next].get(); }
    const utl::StageID& get_next() const { return next; }

    void add_stage(utl::StageID key, std::unique_ptr<Stage> new_stage);
    void set_current_stage(utl::StageID new_current);
    void set_next_stage(utl::StageID new_next);

    void run();
private:
    void handle_stage_transition();

    std::unordered_map<utl::StageID, std::unique_ptr<Stage>> stages{ };
    utl::StageID current;
    utl::StageID next;
    std::array<bool, static_cast<size_t>(utl::KeyFlag::K_TOTAL)> keyState{ };
    Application& m_app;
};
