#pragma once

#include <array>
#include <cstdint>
#include <unordered_map>
#include <memory>

#include "SDL_Interface.hpp"
#include "Stage.hpp"

namespace utl{

class Application;
struct Box;

/**
 * The StageManager holds a collection of, and handles transitions between, Stages.
 * An Application holds a StageMember as a data member, and will call
 * StageManager::run() to run the app; the StageManager holds a reference to the
 * Application that owns it.
 */
class StageManager {
public:
    StageManager(Application& app);

    const std::string& get_current() const { return current; }
    Stage* get_current_stage() { return stages[current].get(); }
    Stage* get_next_stage() { return stages[next].get(); }
    const std::string& get_next() const { return next; }

    // Only ask add_stage() to add (derived) Stages!
    template<typename T, typename... Args>
    void add_stage(const std::string& key, const Box& screen, uint32_t windowID, utl::Renderer& renderer, const Args&... args)
    {
        stages[key] = std::make_unique<T>(screen, windowID, renderer, args...);
    }

    void set_current_stage(const std::string& new_current);
    void set_next_stage(const std::string& new_next);

    void run();
private:
    void handle_stage_transition();

    std::unordered_map<std::string, std::unique_ptr<Stage>> stages{ };
    std::string current;
    std::string next;
    std::array<bool, static_cast<size_t>(utl::KeyFlag::K_TOTAL)> keyState{ };
    Application& m_app;
};

} // namespace utl
