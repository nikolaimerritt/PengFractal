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
	int fractal_main() {
		std::cout.precision(std::numeric_limits<double>::max_digits10 + 1);
		std::cout << "Hello fractal :))" << std::endl;

		const int32_t handle = PengEngine::get().on_engine_initialized().subscribe([&] {
			const peng::shared_ref<const Shader> shader = peng::make_shared<Shader>(
				"resources/fractal/projection.vert",
				"resources/fractal/mandlebrot.frag"
			);

			const auto material = peng::make_shared<Material>(shader);
			
			const auto background = Vector4f(0.9f, 0.3f, 0.4f, 1.0f);
			material->set_parameter("background", background);
			const auto foreground = Vector4(0.3f, 0.9f, 0.6f, 1.0f);
			material->set_parameter("foreground", foreground);

			const Vector2f pos = Vector2f(0.0f, 0.0f);
			PengEngine::get().entity_manager().create_entity<FractalEntity>(Primitives::fullscreen_quad(), material, pos);

			Logger::get().logf(LogSeverity::warning, "doubles enabled??? %d", GLEW_ARB_gpu_shader_fp64);
		});

		PengEngine::get().set_target_fps(60.0f);
		PengEngine::get().set_resolution(Vector2i(600, 400));
		PengEngine::get().start();
		PengEngine::get().on_engine_initialized().unsubscribe(handle);

		return 0;
	}
}
