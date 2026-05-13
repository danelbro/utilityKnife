#include "utl_StageManager.hpp"

#include "utl_Application.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_Stage.hpp"

#include <chrono>
#include <stdexcept>

namespace utl {

StageManager::StageManager(Application* app) : current{""}, next{""}, m_app{app}
{
    std::fill(keyState.begin(), keyState.end(), false);
    LOG("Constructed StageManager\n");
}

const std::string& StageManager::get_current() const
{
    return current;
}

Stage* StageManager::get_current_stage()
{
    return stages[current].get();
}

Stage* StageManager::get_next_stage()
{
    return stages[next].get();
}

const std::string& StageManager::get_next() const
{
    return next;
}

void StageManager::set_current_stage(const std::string& new_current)
{
    current = new_current;
}

void StageManager::set_next_stage(const std::string& new_next)
{
    next = new_next;
}

void StageManager::run()
{
    using namespace std::chrono;
    using namespace std::chrono_literals;
    using utl_duration =
        std::chrono::duration<std::chrono::system_clock::rep,
                              std::chrono::system_clock::period>;

    // Set up for main loop
    // Structure from http://gameprogrammingpatterns.com/game-loop.html

    bool isRunning{true};

    auto t{utl_duration{}};
    const auto dt{utl_duration{10ms}};
    auto frameTimeAccumulator{utl_duration{}};

    auto oldTimePoint{system_clock::now()};
    while (isRunning) {
        auto newTimePoint{system_clock::now()};
        auto frameDuration{newTimePoint - oldTimePoint};
        oldTimePoint = newTimePoint;
        frameTimeAccumulator += frameDuration;
        current = next;

        Stage* current_stage = stages[current].get();

        while (frameTimeAccumulator >= dt) {
            if (!current_stage) {
                throw(std::runtime_error("no stage set!"));
            }

            next = current_stage->handle_input(duration_cast<milliseconds>(t),
                                               duration_cast<milliseconds>(dt),
                                               keyState);
            if (next != current) {
                handle_stage_transition();
                current_stage = nullptr;
                break;
            }

            next = current_stage->update(duration_cast<milliseconds>(t),
                                         duration_cast<milliseconds>(dt));
            if (next != current) {
                handle_stage_transition();
                current_stage = nullptr;
                break;
            }

            frameTimeAccumulator -= dt;
            t += dt;
        }

        if (next == "QUIT") {
            break;
        }

        if (current_stage) {
            current_stage->render(duration_cast<milliseconds>(t),
                                  duration_cast<milliseconds>(dt));
        }
    }
}

void StageManager::handle_stage_transition()
{
    keyState.fill(false);
    m_app->trigger_stage_change(next);
    stages[current].reset(nullptr);
}

}  // namespace utl
