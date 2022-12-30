#include <core/peng_engine.h>
#include <rendering/texture.h>
#include <rendering/primitives.h>
#include <rendering/shader.h>
#include <iostream>
#include <rendering/material.h>
#include <core/logger.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "fractal.h"
#include "fractal_entity.h"

using namespace rendering;
using namespace math;

namespace fractal {
	char const* const FRAGMENT_SHADER = "palette.frag";

	int fractal_main() {
		std::cout.precision(std::numeric_limits<double>::max_digits10 + 1);

		const int32_t handle = PengEngine::get().on_engine_initialized().subscribe([&] {
			const peng::shared_ref<const Shader> shader = peng::make_shared<Shader>(
				"resources/fractal/projection.vert",
				std::format("resources/fractal/{0}", FRAGMENT_SHADER)
			);
			const auto material = peng::make_shared<Material>(shader);
			PengEngine::get().entity_manager().create_entity<FractalEntity>(material);
		});

		PengEngine::get().set_target_fps(60);
		PengEngine::get().set_resolution(Vector2i(1280, 720));
		PengEngine::get().start();
		PengEngine::get().on_engine_initialized().unsubscribe(handle);

		return 0;
	}
}
