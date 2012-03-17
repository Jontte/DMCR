/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <iostream>
#include <algorithm>
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

        std::unique_ptr<Node> low;
        std::unique_ptr<Node> high;

        std::vector<SceneObjectPtr> children;

        Node ()
            : min(0)
            , max(0)
            , split(0)
        {}

        bool is_leaf() {return !children.empty();}

        void build(std::list<SceneObjectPtr> in, size_t level)
        {
            if(in.size() <= KDTREE_LEAF_SIZE)
            {
                // Leaf node!
                children.assign(in.begin(), in.end());
                return;
            }
            level %= 6; // cycle thru 6 values: min x,y,z; max x,y,z

            auto get_value = [=](SceneObjectPtr p) -> float {
                AABB bb = p->aabb();
                if(level < 3)
                    return bb.min()[level];
                else
                    return bb.max()[level%3];
            };

            auto compare_value = [&](const SceneObjectPtr& a, const SceneObjectPtr& b){
                return get_value(a) < get_value(b);
            };

            size_t elems = in.size();
            in.sort(compare_value);

            min = get_value(in.front());
            max = get_value(in.back());

            // move higher half to a new sublist
            std::list<SceneObjectPtr> high_list;

            std::list<SceneObjectPtr>::iterator iter = in.begin();
            std::advance(iter, elems/2);

            high_list.splice(high_list.begin(), in, iter, in.end());

            split = get_value(high_list.front());

            low = dmcr::make_unique<Node>();
            high = dmcr::make_unique<Node>();

            low ->  build(in        , level+1);
            high->  build(high_list , level+1);
        }

        std::list<SceneObjectPtr> getObjects(Ray ray, size_t level, AABB bb)
        {
            if(is_leaf())
                return std::list<SceneObjectPtr>(children.begin(), children.end());

            level %= 6;

            AABB lower(bb);
            AABB higher(bb);

            switch(level)
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
            }
            
            std::list<SceneObjectPtr> ret;
            if( lower.intersects(ray) >= 0)
                ret.splice(ret.end(), low->getObjects(ray, level+1, lower));
            if( higher.intersects(ray) >= 0)
                ret.splice(ret.end(), high->getObjects(ray, level+1, higher));
            return ret;
        }
    } root;
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

    m->root.build(objects, 0);
}

void KDTreeScene::addObject(std::unique_ptr<SceneObject> object)
{
    m_objects.push_back(std::move(object));
}

std::list<SceneObjectPtr> KDTreeScene::intersectionCandidates(
        const Ray &ray) const
{
    float min_value = std::numeric_limits<float>::min();
    float max_value = std::numeric_limits<float>::max();

    auto foo = m->root.getObjects(ray, 0, 
        AABB(Vector3f(min_value,min_value,min_value), 
             Vector3f(max_value,max_value,max_value)));
    //if(!foo.empty())
    //    std::cout << foo.size() << " " << std::flush;
    return foo;
}

}

