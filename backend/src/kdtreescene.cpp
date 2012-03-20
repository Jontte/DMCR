/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <algorithm>
#include <stdexcept>
#include <cassert>
#include "kdtreescene.h"
#include "sceneobject.h"
#include "aabb.h"

namespace dmcr
{

static const unsigned KDTREE_LEAF_SIZE = 4; // Max elements in a leaf node

struct KDTreeScene::impl
{
    class Node
    {
        private:

        typedef std::pair<float,float> Range;
        static float range_overlap(Range r1, Range r2)
        {
            // return how much of r2 is outside r1
            float c = 0;
            if(r2.first > r1.second)
                c += r2.first - r1.second;
            if(r2.second < r1.first)
                c += r1.first - r2.second;
            return c;
        }
        static Range range_combine(Range r1, Range r2)
        {
            return std::make_pair(std::min(r1.first,r2.first), std::max(r1.second, r2.second));
        }
        static float range_length(Range r1)
        {
            assert(r1.second >= r1.first);
            return r1.second - r1.first;
        }

        AABB m_aabb;

        std::unique_ptr<Node> m_low;
        std::unique_ptr<Node> m_high;

        std::vector<SceneObjectPtr> m_children;
        
        public:

        Node () {}

        bool is_leaf() {return !m_low || !m_high;}

        void clear()
        {
            m_children.clear();
            m_low.reset();
            m_high.reset();
        }

        void build(std::list<SceneObjectPtr>& in, uint8_t axis, uint8_t bail = 0)
        {
            if(in.size() <= KDTREE_LEAF_SIZE || bail > 3)
            {
                // Leaf node!
                m_children.assign(in.begin(), in.end());

                std::vector<AABB> aabbs;
                aabbs.resize(in.size());

                std::transform(m_children.begin(), m_children.end(), aabbs.begin(), 
                    [](const SceneObjectPtr& p){return p->aabb();});
                m_aabb = AABB::fromRange(aabbs.begin(), aabbs.end());

                return;
            }

            float min = std::numeric_limits<float>::max();
            float max = std::numeric_limits<float>::lowest();

            for(auto ptr : in)
            {
                const AABB& bb = ptr->aabb();
                if(bb.min()[axis] < min)
                    min = bb.min()[axis];
                if(bb.max()[axis] > max)
                    max = bb.max()[axis];
            }

            auto low_range = std::make_pair(min,min);
            auto high_range = std::make_pair(max,max);

            std::list<SceneObjectPtr> low_list, high_list;

            for(auto ptr : in)
            {
                const AABB& bb = ptr->aabb();
                auto ptr_range = std::make_pair(bb.min()[axis], bb.max()[axis]);

                float overlap_left = range_overlap(low_range, ptr_range);
                float overlap_right = range_overlap(high_range, ptr_range);

                if(overlap_left+range_length(low_range) < overlap_right+range_length(high_range))
                {
                    low_range = range_combine(low_range, ptr_range);
                    low_list.push_back(ptr);
                }
                else
                {
                    high_range = range_combine(high_range, ptr_range);
                    high_list.push_back(ptr);
                }
            }

            in.clear();
            m_low = dmcr::make_unique<Node>();
            m_high = dmcr::make_unique<Node>();

            if(low_list.empty() || high_list.empty())
                bail++;
            else
                bail = 0;

            m_low ->  build(low_list , (axis+1)%3, bail);
            m_high->  build(high_list, (axis+1)%3, bail);

            AABB bb[] = {m_low->m_aabb, m_high->m_aabb};
            m_aabb = AABB::fromRange(bb, bb+2);
        }

        std::list<SceneObjectPtr> getObjects(Ray ray, uint8_t axis)
        {
            if(!m_aabb.intersects(ray).intersects)
                return std::list<SceneObjectPtr>();
            if(is_leaf())
                return std::list<SceneObjectPtr>(m_children.begin(), m_children.end());

            std::list<SceneObjectPtr> ret;

            assert(m_low && m_high);
            
            ret.splice(ret.end(), m_low->getObjects(ray, (axis+1)%3));
            ret.splice(ret.end(), m_high->getObjects(ray, (axis+1)%3));

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
    m->root.clear();
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
    return m->root.getObjects(ray, 0);
}

}

