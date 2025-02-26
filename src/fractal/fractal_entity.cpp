#include "fractal_entity.h"
#include <core/logger.h>
#include <core/peng_engine.h>
#include <rendering/primitives.h>
#include <iostream>

using namespace math;
using namespace input;

namespace fractal {

	FractalEntity::FractalEntity(
		const peng::shared_ref<rendering::Material>& material
	)
		: Entity(true)
		, _mesh(rendering::Primitives::fullscreen_quad()) 
		, _material(material)
		, _zoom(0.1)
		, _complex_centre(Vector2f(0, 0))
	{ }

	void FractalEntity::tick(double delta_time)
	{
		const Vector2i resolution = PengEngine::get().resolution();
		const float height = static_cast<double>(resolution.y) / static_cast<double>(resolution.x);
		_material->set_parameter("height", height);

		_zoom *= zoom_update(delta_time);
		_material->set_parameter("zoom", float(_zoom));

		_complex_centre += displacement(delta_time, _zoom);
		_material->set_parameter("complex_centre", Vector2f(_complex_centre));

		Logger::get().log(LogSeverity::log, std::format("framerate {0} \t centre ({1}, {2}) \t zoom {3}", 
			1.0 / delta_time,
			_complex_centre.x, 
			_complex_centre.y, 
			_zoom)
		);
		
		_material->use();
		_mesh->render();
	}

	Vector2f FractalEntity::displacement(float delta_time, float zoom) {
		const InputManager& input = PengEngine::get().input_manager();
		Vector2f direction = Vector2f(0, 0);

		if (input.get_key(KeyCode::w).is_down()) {
			direction += Vector2f(0, 1);
		}
		if (input.get_key(KeyCode::a).is_down()) {
			direction += Vector2f(-1, 0);
		}
		if (input.get_key(KeyCode::s).is_down()) {
			direction += Vector2f(0, -1);
		}
		if (input.get_key(KeyCode::d).is_down()) {
			direction += Vector2f(1, 0);
		}

		if (direction.magnitude_sqr() < 0.0001) {
			return Vector2f(0, 0);
		}

		Vector2f scaled = direction /= (zoom * std::sqrt(direction.magnitude_sqr()));
		return scaled * delta_time;
	}

	float FractalEntity::zoom_update(float delta_time) {
		const InputManager& input = PengEngine::get().input_manager();
		float zoom = 1.0f;
		if (input[KeyCode::q].is_down())
			zoom *= std::pow(0.5, delta_time);

		if (input[KeyCode::e].is_down())
			zoom /= std::pow(0.5, delta_time);
		return zoom;
	}
}
