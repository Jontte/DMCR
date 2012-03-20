/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <cassert>
#include <vector>
#include "../src/aabb.h"
#include "../src/box.h"
#include "../src/sphere.h"
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

        assert(box.intersects(ray).intersects);

        ray.setDirection(-ray.direction());

        assert(!box.intersects(ray).intersects);
    }

    {
        dmcr::AABB bbs[] = {
            dmcr::AABB(dmcr::Vector3f(0,0,0), dmcr::Vector3f(1,1,1)),
            dmcr::AABB(dmcr::Vector3f(0,0,0), dmcr::Vector3f(1,1,1)),
            dmcr::AABB(dmcr::Vector3f(2,2,2), dmcr::Vector3f(3,3,3))
        };

        assert(dmcr::AABB::fromRange(bbs,bbs+1) == bbs[0]);
        assert(dmcr::AABB::fromRange(bbs+1,bbs+2) == bbs[1]);

        assert(dmcr::AABB::fromRange(bbs,bbs+2) == bbs[0]);
        assert(dmcr::AABB::fromRange(bbs,bbs+2) == bbs[1]);

        assert(dmcr::AABB::fromRange(bbs,bbs+3) == dmcr::AABB(dmcr::Vector3f(0,0,0),dmcr::Vector3f(3,3,3)));
        
    }
    {
        dmcr::Sphere sph[2];
        sph[0].setRadius(5);
        sph[0].setPosition(dmcr::Vector3f(0,0,0));
        sph[1].setRadius(3);
        sph[1].setPosition(dmcr::Vector3f(10,0,0));

        dmcr::AABB bbs[] = {
            sph[0].aabb(),
            sph[1].aabb()
        };
        assert(dmcr::AABB::fromRange(bbs,bbs+2) == dmcr::AABB(dmcr::Vector3f(-5,-5,-5),dmcr::Vector3f(13,5,5)));
    }
}
