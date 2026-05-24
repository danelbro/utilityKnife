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

EditorMain::EditorMain(utl::Application& app)
    : utl::Stage{}, m_app{&app}, m_screen{&app.screen()},
      m_renderer{&app.renderer()}
{
    auto fontPath = std::filesystem::path{"data/Silkscreen.ttf"};
    fonts[utl::Fonts::SILKSCREEN] = utl::Font{utl::createFont(fontPath, 72)};

    entityMap[0] = entities.registerEntity(std::make_unique<utl::TextObject>(
        this, &fonts.at(utl::Fonts::SILKSCREEN),
        utl::Colour{0xff, 0xff, 0xff, 0xff}, "utl_Editor"));
    titleID = 0;
    LOGF("title entity index: %zu\n", titleID);

    for (size_t i{0}; i < 5; ++i) {
        entityMap[i + 1] =
            entities.registerEntity(std::make_unique<utl::TextObject>(
                this, &fonts.at(utl::Fonts::SILKSCREEN),
                utl::Colour{0xff, 0xff, 0xff, 0xff},
                "test_" + std::to_string(i + 1)));
        LOGF("created test_%zu\n", i + 1);
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
        // auto text = std::make_unique<utl::TextObject>(
        //     this, &fonts.at(utl::Fonts::SILKSCREEN),
        //     utl::Colour{0xff, 0x00, 0x00, 0xff}, "tester");
        // text->set_pos({std::rand() % m_screen->w, std::rand() %
        // m_screen->h});
        // testObjectIDs.push_back(entities.registerEntity(std::move(text)));
        // LOGF("Adding a test text object, id %lu", testObjectIDs.back());
        return "EditorMain";
    } else if (key_state[utl::KeyFlag::K_BACKSPACE]) {
        // if (testObjectIDs.size() <= 1)
        //     return "EditorMain";
        // size_t id_to_remove{testObjectIDs.size() - 1};
        // entities.removeEntity(testObjectIDs.at(id_to_remove));
        // testObjectIDs.pop_back();
        // testObjectIDs.shrink_to_fit();
        return "EditorMain";
    } else
        return "EditorMain";
}

static void animateText(utl::EntityPool& entities, size_t id, utl::Box& screen,
                        milliseconds t)
{
    const long double damping{0.25};
    const double omega{0.0005};
    auto& entity = entities.get(id);
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

    std::vector<size_t> keys{};
    keys.reserve(ids.size());
    for (const auto& pair : ids) keys.emplace_back(pair.first);
    new_id = *std::ranges::max_element(keys)
             + 1;  // TODO: probably want to fill gaps
    ids[new_id] = new_idx;
    return new_id;
}

static void removeEntity(utl::EntityPool& entities,
                         std::unordered_map<size_t, size_t>& ids, size_t id)
{
    size_t idx_to_remove = ids.at(id);
    entities.removeEntity(idx_to_remove);
    ids.erase(id);
}

std::string EditorMain::update([[maybe_unused]] milliseconds t,
                               [[maybe_unused]] milliseconds dt)
{
    entities.update(t, dt);
    for (auto& id : entityMap) animateText(entities, id.second, *m_screen, t);
    if (t > 10s && !popped1) {
        removeEntity(entities, entityMap, 3);
        popped1 = true;
    }
    if (t > 20s && !popped2) {
        removeEntity(entities, entityMap, 4);
        popped2 = true;
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
