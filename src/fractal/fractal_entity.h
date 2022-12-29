#pragma once

#include <core/entity.h>
#include <memory/shared_ref.h>
#include <rendering/material.h>
#include <rendering/mesh.h>
#include <math/transform.h>
#include <input/input_manager.h>
#include <input/key_code.h>
#include <input/key_state.h>

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
		double _zoom;
		math::Vector2d _complex_centre;

		math::Vector2d displacement(float delta_time, double zoom);
		double zoom_update(float delta_time);
	};
}

