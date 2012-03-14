#include <cassert>
#include <vector>
#include "../src/box.h"
#include "../src/ray.h"

void test_intersections()
{
    for (int i = 0; i < 10; ++i) {
        dmcr::Box box;
        box.setPosition(dmcr::Vector3f(0, 0, 0));
        box.setExtents(dmcr::Vector3f(rand() % 5 + 1,
                                      rand() % 5 + 1,
                                      rand() % 5 + 1));

        dmcr::Vector3f pos(rand() % 5 + 7,
                           rand() % 5 + 7,
                           rand() % 5 + 7);
        dmcr::Ray ray(pos, (dmcr::Vector3f(0, 0, 0) - pos).normalized());

        assert(box.intersects(ray) > 0.0);

        ray.setDirection(-ray.direction());

        assert(box.intersects(ray) < 0.0);
    }
}
