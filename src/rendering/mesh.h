#pragma once

#include <vector>

#include <GL/glew.h>
#include <math/vector3.h>

namespace rendering
{
	class Mesh
	{
	public:
		Mesh(
			const std::vector<math::Vector3f>& vertices,
			const std::vector<math::Vector3u>& indices,
			const std::vector<math::Vector3f>& colors,
			const std::vector<math::Vector2f>& tex_coords
		);

		~Mesh();

		void render() const;

	private:
		std::vector<float> _vertex_buffer;
		std::vector<math::Vector3u> _index_buffer;

		GLuint _ebo;
		GLuint _vbo;
		GLuint _vao;
	};
}