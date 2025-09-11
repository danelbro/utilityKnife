#pragma once

#include "utl_SDLInterface.hpp"
#include "utl_Stage.hpp"

#include <array>
#include <memory>
#include <unordered_map>

namespace utl {

class Application;
struct Box;

/**
 * The StageManager holds a collection of, and handles transitions between,
 * Stages. An Application holds a StageMember as a data member, and will call
 * StageManager::run() to run the app; the StageManager holds a reference to the
 * Application that owns it.
 */
class StageManager {
public:
    StageManager() = default;
    StageManager(Application* app);

    const std::string& get_current() const;
    Stage* get_current_stage();
    Stage* get_next_stage();
    const std::string& get_next() const;

    // Only ask add_stage() to add (derived) Stages!
    template<typename DerivedStage, typename... Args>
    void add_stage(Application& app, const std::string& key, Args&&... args)
    {
        stages[key] =
            std::make_unique<DerivedStage>(app, std::forward<Args>(args)...);
    }

    void set_current_stage(const std::string& new_current);
    void set_next_stage(const std::string& new_next);

    void run();

private:
    void handle_stage_transition();

private:
    std::unordered_map<std::string, std::unique_ptr<Stage>> stages{};
    std::string current{};
    std::string next{};
    std::array<bool, static_cast<size_t>(utl::KeyFlag::K_TOTAL)> keyState{};
    Application* m_app{nullptr};
};

}  // namespace utl
