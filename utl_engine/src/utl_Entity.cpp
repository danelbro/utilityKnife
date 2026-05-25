#include "utl_Entity.hpp"

#include <algorithm>
#include <cstddef>

namespace utl {

void EntityPool::update(std::chrono::milliseconds t,
                        std::chrono::milliseconds dt)
{
    std::ranges::for_each(m_entities,
                          [=](auto& entity) { entity->update(t, dt); });
}

void EntityPool::render(utl::Renderer& renderer)
{
    std::ranges::for_each(m_entities,
                          [&](auto& entity) { entity->render(renderer); });
}

size_t EntityPool::registerEntity(std::unique_ptr<Entity>&& entity)
{
    m_entities.emplace_back(std::move(entity));
    return m_entities.size() - 1;
}

void EntityPool::removeEntity(size_t entityToEraseIDX)
{
    size_t back_idx = m_entities.size() - 1;

    if (m_entities.empty())
        return;

    if (entityToEraseIDX > back_idx)
        return;

    if (entityToEraseIDX == back_idx) {
        m_entities.pop_back();
        return;
    }

    std::swap(m_entities.at(entityToEraseIDX), m_entities.at(back_idx));
    m_entities.pop_back();
}

std::unique_ptr<Entity>& EntityPool::get(size_t id)
{
    if (m_entities.empty())
        throw;
    return m_entities.at(id);
}

void EntityPool::for_each(
    const std::function<void(std::unique_ptr<Entity>& e)>& f)
{
    std::ranges::for_each(m_entities, f);
}
}  // namespace utl
