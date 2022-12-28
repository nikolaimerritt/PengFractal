#include "fractal_entity.h"
#include <core/logger.h>
#include <core/peng_engine.h>

using namespace math;

namespace fractal {

	FractalEntity::FractalEntity(
		const peng::shared_ref<const rendering::Mesh>& mesh,
		const peng::shared_ref<rendering::Material>& material,
		const Vector2f& pos_px
	)
		: Entity(true)
		, _mesh(mesh)
		, _material(material)
		, _time_elapsed(0.0)
	{ }

	void FractalEntity::tick(double delta_time)
	{
		_time_elapsed += delta_time;
		const Vector2i resolution = PengEngine::get().resolution();
		const float height = static_cast<float>(resolution.y) / static_cast<float>(resolution.x);
		const float zoom = std::pow(0.9, _time_elapsed);

		_material->set_parameter("height", height);
		_material->set_parameter("zoom", zoom);
		_material->use();
		_mesh->render();
	

		Logger::get().log(LogSeverity::log, std::format("Framerate: {0}", 1.0 / delta_time));
	}
}
