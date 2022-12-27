#include <core/peng_engine.h>
#include <rendering/texture.h>
#include <rendering/primitives.h>
#include <rendering/shader.h>
#include <iostream>
#include <rendering/material.h>

#include "fractal.h"
#include "fractal_entity.h"

using namespace rendering;
using namespace math;

namespace fractal {
	int fractal_main() {
		std::cout << "Hello fractal :))" << std::endl;

		const int32_t handle = PengEngine::get().on_engine_initialized().subscribe([&] {
			const peng::shared_ref<const Shader> shader = peng::make_shared<Shader>(
				"src/fractal/shader/projection.vert",
				"src/fractal/shader/unlit.frag"
			);

			const auto material = peng::make_shared<Material>(shader);
			const auto colour = Vector4f(0.5f, 0.3f, 0.4f, 1.0f);
			material->set_parameter("base_color", colour);
			const Vector2f pos = Vector2f(0.0f, 0.0f);
			PengEngine::get().entity_manager().create_entity<FractalEntity>(Primitives::fullscreen_quad(), material, pos);

		});

		PengEngine::get().set_target_fps(60.0f);
		PengEngine::get().start();
		PengEngine::get().on_engine_initialized().unsubscribe(handle);

		return 0;
	}
}
