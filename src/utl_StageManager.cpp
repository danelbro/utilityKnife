#include "utl_StageManager.h"

#include "utl_Application.h"
#include "utl_Stage.h"

#include <chrono>
#include <stdexcept>

namespace utl {

StageManager::StageManager(Application& app) : current{""}, next{""}, m_app{app}
{
    std::fill(keyState.begin(), keyState.end(), false);
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
    using std::chrono::duration;
    using std::chrono::high_resolution_clock;

    // Set up for main loop
    // Structure from http://gameprogrammingpatterns.com/game-loop.html

    bool isRunning{true};

    double t{0.0};
    const double dt{0.01};

    auto currentTime{high_resolution_clock::now()};
    double accumulator{0.0};
    while (isRunning) {
        current = next;
        Stage* current_stage = stages[current].get();

        auto newTime{high_resolution_clock::now()};
        auto frameTime{duration<double>(newTime - currentTime)};
        currentTime = newTime;

        accumulator += frameTime.count();

        while (accumulator >= dt) {
            if (!current_stage) {
                throw(std::runtime_error("no stage set!"));
            }

            next = current_stage->handle_input(t, dt, keyState);

            if (next != current) {
                handle_stage_transition();
                current_stage = nullptr;
                break;
            }

            next = current_stage->update(t, dt);

            if (next != current) {
                handle_stage_transition();
                current_stage = nullptr;
                break;
            }

            accumulator -= dt;
            t += dt;
        }

        if (next == "QUIT") {
            break;
        }

        if (current_stage) {
            current_stage->render(t, dt);
        }
    }
}

void StageManager::handle_stage_transition()
{
    keyState.fill(false);
    m_app.trigger_stage_change(next);
    stages[current].reset(nullptr);
}

}  // namespace utl
