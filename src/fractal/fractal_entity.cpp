#include "fractal_entity.h"

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
	{
		const Vector2f resolution = PengEngine::get().resolution();
	}

	void FractalEntity::tick(double delta_time)
	{
		_material->use();
		_mesh->render();
	}
}
