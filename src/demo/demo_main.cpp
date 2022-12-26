#include <core/peng_engine.h>
#include <rendering/texture.h>

#include "blob_entity.h"

using namespace math;
using namespace rendering;

namespace demo
{
    std::vector<Vector3f> vertices =
    {
        Vector3f(-0.5f, -0.5f, -0.5f),
	    Vector3f(0.5f, -0.5f, -0.5f),
	    Vector3f(0.5f, 0.5f, -0.5f),
	    Vector3f(-0.5f, 0.5f, -0.5f),

        Vector3f(-0.5f, -0.5f, 0.5f),
        Vector3f(0.5f, -0.5f, 0.5f),
        Vector3f(0.5f, 0.5f, 0.5f),
        Vector3f(-0.5f, 0.5f, 0.5f),

        Vector3f(-0.5f, -0.5f, -0.5f),
        Vector3f(-0.5f, 0.5f, -0.5f),
        Vector3f(-0.5f, 0.5f, 0.5f),
        Vector3f(-0.5f, -0.5f, 0.5f),

        Vector3f(0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, 0.5f),
        Vector3f(0.5f, -0.5f, 0.5f),

        Vector3f(-0.5f, 0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, -0.5f),
        Vector3f(0.5f, 0.5f, 0.5f),
        Vector3f(-0.5f, 0.5f, 0.5f),

        Vector3f(-0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, -0.5f, -0.5f),
        Vector3f(0.5f, -0.5f, 0.5f),
        Vector3f(-0.5f, -0.5f, 0.5f),
    };

    std::vector<Vector3u> indices =
    {
        Vector3u(0, 1, 3),
    	Vector3u(3, 1, 2),

        Vector3u(4, 5, 7),
        Vector3u(7, 5, 6),

        Vector3u(8, 9, 11),
        Vector3u(11, 9, 10),

        Vector3u(12, 13, 15),
        Vector3u(15, 13, 14),

        Vector3u(16, 17, 19),
        Vector3u(19, 17, 18),

        Vector3u(20, 21, 23),
        Vector3u(23, 21, 22),
    };

    std::vector<Vector3f> colors =
    {
        Vector3f(1, 0, 0),
        Vector3f(0, 1, 0),
        Vector3f(0, 0, 1),
        Vector3f(1, 1, 0),
        Vector3f(0, 1, 1),
        Vector3f(1, 0, 1),
        Vector3f(1, 1, 1),
        Vector3f(0, 0, 0),

        Vector3f(1, 1, 1),
        Vector3f(1, 0, 1),
        Vector3f(1, 1, 0),
        Vector3f(0, 1, 1),
        Vector3f(0, 1, 0),
        Vector3f(0, 0, 1),
        Vector3f(1, 0, 0),
        Vector3f(0, 0, 0),

        Vector3f(0, 0, 1),
        Vector3f(0, 1, 0),
        Vector3f(1, 0, 0),
        Vector3f(0, 1, 1),
        Vector3f(1, 0, 1),
        Vector3f(1, 1, 0),
        Vector3f(0, 0, 0),
        Vector3f(1, 1, 1),

        Vector3f(0, 0, 0),
        Vector3f(1, 1, 1),
        Vector3f(0, 1, 1),
        Vector3f(1, 0, 1),
        Vector3f(1, 1, 0),
        Vector3f(1, 0, 0),
        Vector3f(0, 1, 0),
        Vector3f(0, 0, 1),
    };

    std::vector<Vector2f> tex_coords =
    {
        Vector2f(0, 0),
	    Vector2f(1, 0),
	    Vector2f(1, 1),
	    Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
        Vector2f(0, 0),
        Vector2f(1, 0),
        Vector2f(1, 1),
        Vector2f(0, 1),
    };

    class FPSEntity : public Entity
    {
    public:
        FPSEntity()
            : Entity(true)
        { }

        virtual void tick(double delta_time) override
        {
            printf("Frametime = %.02fms\n", delta_time * 1000);
        }
    };

    class ShutdownEntity : public Entity
    {
    public:
        ShutdownEntity(double max_age)
            : Entity(true)
			, _age(0)
			, _max_age(max_age)
        { }

        virtual void tick(double delta_time) override
        {
            _age += delta_time;
            if (_age >= _max_age)
            {
                PengEngine::get().request_shutdown();
            }
        }

    private:
        double _age;
        double _max_age;
    };

    int demo_main()
    {
	    const int32_t handle = PengEngine::get().on_engine_initialized().subscribe([&]
	    {
		    const auto shader = peng::make_shared<Shader>(
			    "resources/shaders/demo/blob.vert",
			    "resources/shaders/demo/blob.frag"
		    );

		    const auto texture = peng::make_shared<Texture>("wall",
		        "resources/textures/demo/wall.jpg"
		    );

		    auto material = peng::make_shared<Material>(shader);
		    material->set_parameter("color_tex", texture);

		    const auto mesh = peng::make_shared<Mesh>("cube", vertices, indices, colors, tex_coords);

		    const Vector2i blob_grid(5, 4);

		    for (int32_t blob_x = 0; blob_x < blob_grid.x; blob_x++)
		    {
			    for (int32_t blob_y = 0; blob_y < blob_grid.y; blob_y++)
			    {
				    const Vector2f pos = Vector2f(blob_x - (blob_grid.x - 1) / 2.0f, blob_y - (blob_grid.y - 1) / 2.0f)
					    * 300;
				    PengEngine::get().entity_manager().create_entity<BlobEntity>(mesh, material, pos, 175.0f);
			    }
		    }
	    }, "demo_start");

        PengEngine::get().set_target_fps(60);
        PengEngine::get().start();
        PengEngine::get().on_engine_initialized().unsubscribe(handle);

        return 0;
    }
}