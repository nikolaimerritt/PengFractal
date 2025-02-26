#include "entity_manager.h"

#include <cassert>
#include <utils/vectools.h>

#include "logger.h"

void EntityManager::tick(double delta_time)
{
	flush_pending_adds();
	tick_entities(delta_time);
	flush_pending_kills();
}

void EntityManager::shutdown()
{
	Logger::get().log(LogSeverity::log, "Destroying all entities");

	for (peng::shared_ref<Entity>& entity : _entities)
	{
		// TODO: add a destroy reason (explicit / shutdown)
		entity->pre_destroy();
	}

	_pending_adds.clear();
	_entities.clear();
}

void EntityManager::destroy_entity(const peng::weak_ptr<Entity>& entity)
{
	assert(entity.valid());

	// TODO: check if entity is already queued for destruction
	_pending_kills.push_back(entity);
}

EntityState EntityManager::get_entity_state(const peng::weak_ptr<Entity>& entity) const
{
	if (vectools::contains(_pending_kills, entity))
	{
		return EntityState::pending_kill;
	}

	if (const peng::shared_ptr<Entity> strong_entity = entity.lock())
	{
		const peng::shared_ref<Entity> entity_ref = strong_entity.to_shared_ref();
		if (vectools::contains(_entities, entity_ref))
		{
			return EntityState::valid;
		}
		if (vectools::contains(_pending_adds, entity_ref))
		{
			return EntityState::pending_add;
		}
	}

	return EntityState::invalid;
}

void EntityManager::tick_entities(double delta_time)
{
	for (const peng::shared_ref<Entity>& entity : _entities)
	{
		if (entity->can_tick() && entity->is_active())
		{
			entity->tick(delta_time);
		}
	}
}

void EntityManager::flush_pending_adds()
{
	const std::vector staged_adds(std::move(_pending_adds));

	for (const peng::shared_ref<Entity>& entity : staged_adds)
	{
		_entities.push_back(entity);
	}

	for (const peng::shared_ref<Entity>& entity : staged_adds)
	{
		entity->post_create();
	}
}

void EntityManager::flush_pending_kills()
{
	auto kill_in_buffer = [&](std::vector<peng::shared_ref<Entity>>& entities, bool exists_yet)
	{
		for (int32_t entity_index = static_cast<int32_t>(entities.size() - 1); entity_index >= 0; entity_index--)
		{
			peng::shared_ref<Entity>& entity = entities[entity_index];
			peng::weak_ptr<Entity> weak_entity = entity;

			if (vectools::contains(_pending_kills, weak_entity))
			{
				if (exists_yet)
				{
					entity->pre_destroy();
				}

				entities.erase(entities.begin() + entity_index);

				if (weak_entity.valid())
				{
					Logger::get().log(LogSeverity::warning, "Entity still exists after kill, potential leak");
				}
			}
		}
	};

	kill_in_buffer(_entities, true);
	kill_in_buffer(_pending_adds, false);

	_pending_kills.clear();
}