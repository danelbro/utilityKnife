#include "utl_Entity.hpp"

#include <ranges>

namespace utl {

void EntityPool::update(std::chrono::milliseconds t, std::chrono::milliseconds dt)
{
	std::ranges::for_each(m_entities, [=](auto& entity) { entity->update(t, dt); });
}

void EntityPool::render(utl::Renderer& renderer)
{
	std::ranges::for_each(m_entities, [&](auto& entity) { entity->render(renderer); });
}

size_t EntityPool::registerEntity(std::unique_ptr<Entity>&& entity)
{
	m_entities.push_back(std::move(entity));
	size_t idx{ m_entities.size() - 1 };
	indexMap[idx] = idx;
	return idx;
}

void EntityPool::removeEntity(size_t mid_id)
{
	size_t mid_idx = indexMap[mid_id];
	size_t back_id{};
	for (const auto& e : indexMap) {
		if (e.second == m_entities.size() - 1) {
			back_id = e.first;
		}
	}

	std::swap(m_entities.at(mid_idx), m_entities.back());
	m_entities.pop_back();
	m_entities.shrink_to_fit();

	indexMap[back_id] = mid_idx;
	indexMap.erase(mid_id);
}

std::unique_ptr<Entity>& EntityPool::get(size_t id)
{
	return m_entities.at(indexMap[id]);
}

void EntityPool::for_each(const std::function<void(std::unique_ptr<Entity>& e)>& f)
{
	std::ranges::for_each(m_entities, f);
}
} // namespace utl