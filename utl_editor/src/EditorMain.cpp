#include "EditorMain.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "flags.hpp"
#include "utl_Application.hpp"
#include "utl_Entity.hpp"
#include "utl_SDLInterface.hpp"
#include "utl_TextObject.hpp"

using EntityId = size_t;
using namespace std::chrono;
using namespace std::chrono_literals;

static void animateText(utl::EntityPool& entities, size_t id, size_t index,
                        utl::Box& screen, milliseconds t);
static std::size_t addEntity(std::unique_ptr<utl::Entity>&& new_entity,
                             utl::EntityPool& entities,
                             std::unordered_map<size_t, size_t>& ids);
static void removeEntity(utl::EntityPool& entities,
                         std::unordered_map<size_t, size_t>& ids, size_t id);

EditorMain::EditorMain(utl::Application& app)
    : utl::Stage{}, m_app{&app}, m_screen{&app.screen()},
      m_renderer{&app.renderer()}
{
    auto fontPath = std::filesystem::path{"data/Silkscreen.ttf"};
    fonts[utl::Fonts::SILKSCREEN] = utl::Font{utl::createFont(fontPath, 72)};

    titleID = addEntity(std::make_unique<utl::TextObject>(
                            this, &fonts.at(utl::Fonts::SILKSCREEN),
                            utl::Colour{0xff, 0xff, 0xff, 0xff}, "utl_Editor"),
                        entities, entityMap);
    LOGF("title entity index: %zu\n", entityMap[titleID]);

    for (size_t i{0}; i < 5; ++i) {
        size_t testObjId =
            addEntity(std::make_unique<utl::TextObject>(
                          this, &fonts.at(utl::Fonts::SILKSCREEN),
                          utl::Colour{0xff, 0xff, 0xff, 0xff},
                          "test_" + std::to_string(i + 1)),
                      entities, entityMap);
        LOGF("created test_%zu\n", testObjId);
    }
}

std::string
EditorMain::handle_input([[maybe_unused]] milliseconds t,
                         [[maybe_unused]] milliseconds dt,
                         std::array<bool, utl::KeyFlag::K_TOTAL>& key_state)
{
    utl::process_input(*m_screen, m_app->window().ID(), key_state);

    if (key_state[utl::KeyFlag::QUIT] || key_state[utl::KeyFlag::K_ESCAPE])
        return "QUIT";
    else if (key_state[utl::KeyFlag::K_SPACE]) {
        return "EditorMain";
    } else if (key_state[utl::KeyFlag::K_BACKSPACE]) {
        return "EditorMain";
    } else
        return "EditorMain";
}

static void animateText(utl::EntityPool& entities, size_t id, size_t index,
                        utl::Box& screen, milliseconds t)
{
    const long double damping{0.25};
    const double omega{0.0005};
    auto& entity = entities.get(index);
    if (!entity)
        return;
    try {
        auto& to = dynamic_cast<utl::TextObject&>(*entity);
        to.set_x_pos(
            (static_cast<int>(t.count() * damping
                              + (to.size().w * static_cast<double>(id)))
             % static_cast<int>(screen.w + to.size().w))
            - to.size().w);
        to.set_y_pos(
            ((std::sin(omega * static_cast<double>(t.count())) + 1.0) / 2.0)
                * (screen.h - to.size().h)
            + (to.size().h * static_cast<double>(id)));
    }
    catch (std::bad_cast&) {
        return;
    }
}

static std::size_t addEntity(std::unique_ptr<utl::Entity>&& new_entity,
                             utl::EntityPool& entities,
                             std::unordered_map<size_t, size_t>& ids)
{
    size_t new_idx = entities.registerEntity(std::move(new_entity));
    size_t new_id{};

    if (ids.size() == 0) {
        new_id = 0;
    } else if (ids.size() == 1) {
        if (ids.begin()->first == 1)
            new_id = 0;
        else
            new_id = 1;
    } else {
        std::vector<size_t> keys{};
        keys.reserve(ids.size());
        for (const auto& pair : ids) {
            keys.emplace_back(pair.first);
        }

        // look for gaps in the ids list we can reregister. Otherwise,
        // incrememnt the largest id by 1
        std::ranges::sort(keys);
        for (size_t j{1}; j < keys.size(); ++j) {
            size_t i{j - 1};
            if (keys[j] - keys[i] > 1) {
                new_id = keys[i] + 1;
                break;
            } else if (j == keys.size() - 1)
                new_id = *std::ranges::max_element(keys) + 1;
        }
    }

    ids[new_id] = new_idx;
    return new_id;
}

static void removeEntity(utl::EntityPool& entities,
                         std::unordered_map<size_t, size_t>& ids, size_t id)
{
    size_t idx_to_remove = ids.at(id);
    size_t back_idx = entities.size() - 1;
    size_t back_id{};
    for (const auto& pair : ids) {
        if (pair.second == back_idx) {
            back_id = pair.first;
            break;
        }
    }

    entities.removeEntity(idx_to_remove);

    ids.at(back_id) = idx_to_remove;
    ids.erase(id);
}

std::string EditorMain::update([[maybe_unused]] milliseconds t,
                               [[maybe_unused]] milliseconds dt)
{
    entities.update(t, dt);
    for (auto& id : entityMap)
        animateText(entities, id.first, id.second, *m_screen, t);
    if (t > 5s && !popped1) {
        removeEntity(entities, entityMap, 3);
        popped1 = true;
    }
    if (t > 10s && !popped2) {
        addEntity(std::make_unique<utl::TextObject>(
                      this, &fonts.at(utl::Fonts::SILKSCREEN),
                      utl::Colour{0xff, 0xff, 0xff, 0xff}, "test_3"),
                  entities, entityMap);
        popped2 = true;
    }
    if (t > 15s && !popped3) {
        addEntity(std::make_unique<utl::TextObject>(
                      this, &fonts.at(utl::Fonts::SILKSCREEN),
                      utl::Colour{0xff, 0xff, 0xff, 0xff}, "test_6"),
                  entities, entityMap);
        popped3 = true;
    }
    return "EditorMain";
}

void EditorMain::render([[maybe_unused]] milliseconds t,
                        [[maybe_unused]] milliseconds dt)
{
    utl::clearScreen(*m_renderer);
    entities.render(*m_renderer);
    utl::presentRenderer(*m_renderer);
}
