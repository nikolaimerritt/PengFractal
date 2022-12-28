#pragma once

#include <core/entity.h>
#include <memory/shared_ref.h>
#include <rendering/material.h>
#include <rendering/mesh.h>
#include <math/transform.h>

namespace fractal {

	class FractalEntity : public Entity
	{
	public:
		FractalEntity(
			const peng::shared_ref<const rendering::Mesh>& mesh,
			const peng::shared_ref<rendering::Material>& material,
			const math::Vector2f& pos_px
		);

		virtual void tick(double delta_time) override;

	private:
		peng::shared_ref<const rendering::Mesh> _mesh;
		peng::shared_ref<rendering::Material> _material;
		double _time_elapsed;
	};
}

