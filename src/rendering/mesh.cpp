#include "mesh.h"

#include <utils/utils.h>
#include <utils/strtools.h>
#include <utils/vectools.h>
#include <core/logger.h>

using namespace rendering;
using namespace math;

Mesh::Mesh(
	const std::string& name,
	const std::vector<Vector3f>& vertices,
	const std::vector<Vector3u>& indices,
	const std::vector<Vector3f>& colors,
	const std::vector<Vector2f>& tex_coords
)
	: _name(name)
	, _index_buffer(indices)
	, _num_indices(static_cast<GLuint>(indices.size() * 3))
{
	if constexpr (Logger::enabled())
	{
		Logger::get().logf(LogSeverity::log, "Building mesh '%s'", name.c_str());

		if (!colors.empty() && colors.size() != vertices.size())
		{
			Logger::get().logf(LogSeverity::warning,
				"Mesh '%s' has %d vertices but %d vertex colors",
				name.c_str(), vertices.size(), colors.size()
			);
		}

		if (!tex_coords.empty() && tex_coords.size() != vertices.size())
		{
			Logger::get().logf(LogSeverity::warning,
				"Mesh '%s' has %d vertices but %d texture coordinates",
				name.c_str(), vertices.size(), tex_coords.size()
			);
		}
	}

	_vertex_buffer.resize(vertices.size(), Vertex(Vector3f::zero(), Vector3f::one(), Vector2f::zero()));
	for (size_t vert_index = 0; vert_index < vertices.size(); vert_index++)
	{
		_vertex_buffer[vert_index].position = vertices[vert_index];

		if (vert_index < colors.size())
		{
			_vertex_buffer[vert_index].color = colors[vert_index];
		}

		if (vert_index < tex_coords.size())
		{
			_vertex_buffer[vert_index].tex_coord = tex_coords[vert_index];
		}
	}

	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenVertexArrays(1, &_vao);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vectools::buffer_size(_vertex_buffer), _vertex_buffer.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vectools::buffer_size(_index_buffer), _index_buffer.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
	glEnableVertexAttribArray(2);
}

Mesh::~Mesh()
{
	Logger::get().logf(LogSeverity::log, "Destroying mesh '%s'", _name.c_str());

	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	glDeleteVertexArrays(1, &_vao);
}

void Mesh::render() const
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

