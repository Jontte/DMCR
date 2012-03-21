/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "renderer.h"
#include <fstream>
#include <iostream>
#include "random.h"

dmcr::RenderResult::RenderResult(uint16_t left, uint16_t right,
                                 uint16_t top, uint16_t bottom) :
    m_left(left),
    m_right(right),
    m_top(top),
    m_bottom(bottom),
    m_width(right - left + 1),
    m_height(bottom - top + 1),
    m_iterations_done(0),
    m_data(m_width * m_height)
{
}

void dmcr::RenderResult::saveImage(const std::string& file_name) const
{
    std::ofstream file;
    
    file.open(file_name.c_str(), std::ios::out | std::ios::binary);
    
    if (!file.is_open())
        throw dmcr::RenderResultException(std::string("Could not open file ") + 
                                          file_name);
    
    // Write PPM file header
    file << "P6 " << m_width << " " << m_height << " " << 255 << "\n";

    // Write pixel array
    for (int i = 0; i < m_width * m_height; ++i) {
        // Convert floats to chars
        char c[3] = { (char)(m_data[i].r() * 255), 
                      (char)(m_data[i].g() * 255),
                      (char)(m_data[i].b() * 255) };
        file.write(c, 3);
    }
    
    file.close();
}

void dmcr::RenderResult::blendInto(dmcr::RenderResultPtr result,
                                   uint32_t count)
{
    if (result->left() > left() || result->right() < right() ||
        result->top() > top() || result->bottom() < bottom())
        throw RenderResultException("Cannot copy into subset result");
    if (result->left() != 0 || result->top() != 0)
        throw RenderResultException("UNIMPLEMENTED copying into non-result");

    float fc = (float)count;
    for (uint16_t y = top(); y <= bottom(); ++y) {
        for (uint16_t x = left(); x <= right(); ++x) {
            const auto& c = pixel(x-left(), y-top());
            const auto& r = result->pixel(x, y);
            float _r = ((r.r() * fc) + c.r()) / (fc + 1.0);
            float _g = ((r.g() * fc) + c.g()) / (fc + 1.0);
            float _b = ((r.b() * fc) + c.b()) / (fc + 1.0);
            result->setPixel(x, y, dmcr::Color{_r, _g, _b});
        }
    }
}


dmcr::RenderResultPtr dmcr::Renderer::render(uint16_t h_res, uint16_t v_res, 
                                             uint16_t left, uint16_t right, 
                                             uint16_t top, uint16_t bottom) const
{
    // By default render the whole image
    if (right == 0)
        right = h_res - 1;
    if (bottom == 0)
        bottom = v_res - 1;

    dmcr::RenderResultPtr result = std::make_shared<dmcr::RenderResult>(
        left, right, top, bottom);

    for (uint16_t y = top; y <= bottom; ++y) {
        for (uint16_t x = left; x <= right; ++x) {
            float fx = (float)x / (float)h_res;
            float fy = (float)y / (float)v_res;
            fx += m_rng.random() / h_res;
            fy += m_rng.random() / v_res;
            dmcr::Color c = midfunc(m_scene->camera().ray(fx, fy));
            result->setPixel(x - left, y - top, c);
        }
    }
    
    return result;
}

dmcr::Color dmcr::Renderer::midfunc(dmcr::Ray ray) const
{
    return iterator(ray);
}

// getting pixel color
dmcr::Color dmcr::Renderer::iterator(dmcr::Ray ray, int iterations) const
{
    if (iterations == 20)
        return { 0.0, 0.0, 0.0 };
    RaycastResult rr = m_scene->shootRay(ray);
    dmcr::Color c = { 0, 0, 0 };
    
    SceneObjectPtr obj = rr.object();

    if (obj != nullptr) {
        if (obj->light())
            c = obj->color();
        else {
            dmcr::Vector3f refl = ray.direction() - 2 * rr.normal().dot(ray.direction()) *
                rr.normal();
            dmcr::Vector3f random = m_rng.random_vector();
            if (random.dot(rr.normal()) < 0.0)
                random = -random;

            float blur = rr.object()->blur();
            dmcr::Vector3f dir = (blur * random + 
                (1.0f - blur) * refl).normalized();
            
            dmcr::Ray new_ray(rr.intersectionPoint(), dir);
                              
            c = dmcr::Color(
                iterator(new_ray, iterations + 1) * rr.object()->color());
        }
    }
    return c;
}
