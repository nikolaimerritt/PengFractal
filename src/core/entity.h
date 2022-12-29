#pragma once

#include <memory>

class Entity : public std::enable_shared_from_this<Entity>
{
public:
	explicit Entity(bool can_tick);
	virtual ~Entity() = default;

	virtual void tick(double delta_time);
	virtual void post_create() { }
	virtual void pre_destroy() { }

	[[nodiscard]] bool can_tick() const noexcept
	{
		return _can_tick;
	}

protected:
	bool _can_tick;
};

enum class EntityState
{
	invalid,
	valid,
	pending_add,
	pending_kill
};