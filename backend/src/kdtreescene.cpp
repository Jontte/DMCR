/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "kdtreescene.h"
#include "sceneobject.h"
#include "aabb.h"

namespace dmcr
{

static const unsigned KDTREE_LEAF_SIZE = 4; // Max elements in a leaf node

struct KDTreeScene::impl
{
    struct Node
    {
        float min;
        float max;
        float split;

        uint8_t axis; // 0-5 inclusive

        std::unique_ptr<Node> low;
        std::unique_ptr<Node> high;

        std::vector<SceneObjectPtr> children;

        Node ()
            : min(0)
            , max(0)
            , split(0)
            , axis(0)
        {}

        bool is_leaf() {return !children.empty();}

        void build(std::list<SceneObjectPtr> in)
        {
            if(in.size() <= KDTREE_LEAF_SIZE)
            {
                // Leaf node!
                children.assign(in.begin(), in.end());
                return;
            }

            auto get_value = [&](SceneObjectPtr p) -> float {
                if(axis < 3)
                    return p->aabb().min()[axis];
                else
                    return p->aabb().max()[axis%3];
            };

            auto compare_value = [&](const SceneObjectPtr& a, const SceneObjectPtr& b){
                return get_value(a) < get_value(b);
            };

            uint8_t best_axis = 0;
            float largest_diff = 0;

            for(axis = 0; axis < 6; axis++)
            {
                // pick axis with largest difference
                auto minmax = std::minmax_element( in.begin(), in.end(), compare_value);

                auto minmax_pair = 
                    std::make_pair(
                        get_value(*(minmax.first)),
                        get_value(*(minmax.second))
                );

                float d = fabs(minmax_pair.second-minmax_pair.first);
                if( d > largest_diff)
                {
                    best_axis = axis;
                    largest_diff = d;
                    min = minmax_pair.first;
                    max = minmax_pair.second;
                }
            }
            if(largest_diff < 0.001f)
            {
                // Unable to split further, make leaf node
                children.assign(in.begin(), in.end());
                return;
            }
            axis = best_axis;
            //std::cout << "Chose " << (int)axis << " sep: " << max-min << " with " << in.size() << " children" << std::endl;

            // Find median
            size_t elems = in.size();
            in.sort(compare_value);
            
            std::list<SceneObjectPtr>::iterator iter = in.begin();
            std::advance(iter, elems/2);
            
            // move higher half to a new sublist
            std::list<SceneObjectPtr> high_list;

            high_list.splice(high_list.begin(), in, iter, in.end());
            split = get_value(high_list.front());

            low = dmcr::make_unique<Node>();
            high = dmcr::make_unique<Node>();

            low ->  build(in       );
            high->  build(high_list);
        }

        std::list<SceneObjectPtr> getObjects(Ray ray, const AABB& bb)
        {
            if(is_leaf())
                return std::list<SceneObjectPtr>(children.begin(), children.end());

            AABB lower(bb);
            AABB higher(bb);

            switch(axis)
            {
                case 0:
                    lower.setMin(lower.min().setX(min));
                    higher.setMin(higher.min().setX(split));
                break;
                case 1:
                    lower.setMin(lower.min().setY(min));
                    higher.setMin(higher.min().setY(split));
                break;
                case 2:
                    lower.setMin(lower.min().setZ(min));
                    higher.setMin(higher.min().setZ(split));
                break;
                case 3:
                    lower.setMax(lower.max().setX(split));
                    higher.setMax(higher.max().setX(max));
                break;
                case 4:
                    lower.setMax(lower.max().setY(split));
                    higher.setMax(higher.max().setY(max));
                break;
                case 5:
                    lower.setMax(lower.max().setZ(split));
                    higher.setMax(higher.max().setZ(max));
                break;
                default:
                    throw std::runtime_error("KDTree axis > 5 makes no sense");
                break;
            }
            
            std::list<SceneObjectPtr> ret;
            if( lower.intersects(ray).intersects )
                ret.splice(ret.end(), low->getObjects(ray, lower));
            if( higher.intersects(ray).intersects )
                ret.splice(ret.end(), high->getObjects(ray, higher));
            return ret;
        }
    } root;

    // Minimal bounding volume that covers the whole tree
    AABB bounding;
};

KDTreeScene::KDTreeScene() : 
    m(new impl)
{
}
KDTreeScene::~KDTreeScene()
{
    delete m;
}

void KDTreeScene::beginAddObjects()
{
}

void KDTreeScene::endAddObjects()
{
    std::list<SceneObjectPtr> objects;
    for (auto& i : m_objects)
        objects.push_back(i.get());

    m->root.build(objects);

    std::vector<AABB> aabbs;
    aabbs.resize(m_objects.size());

    std::transform(m_objects.begin(), m_objects.end(), aabbs.begin(), [](const std::unique_ptr<SceneObject>& p){return p->aabb();});
    m->bounding = AABB::fromRange(aabbs.begin(), aabbs.end());
}

void KDTreeScene::addObject(std::unique_ptr<SceneObject> object)
{
    m_objects.push_back(std::move(object));
}

std::list<SceneObjectPtr> KDTreeScene::intersectionCandidates(
        const Ray &ray) const
{
    return m->root.getObjects(ray, m->bounding);
}

}

