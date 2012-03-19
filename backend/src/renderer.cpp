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
        char c[3] = { (char)(m_data[i].r * 255), 
                      (char)(m_data[i].g * 255),
                      (char)(m_data[i].b * 255) };
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
            float _r = ((r.r * fc) + c.r) / (fc + 1.0);
            float _g = ((r.g * fc) + c.g) / (fc + 1.0);
            float _b = ((r.b * fc) + c.b) / (fc + 1.0);
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

    RNG rng;

    for (uint16_t y = top; y <= bottom; ++y) {
        for (uint16_t x = left; x <= right; ++x) {
            float fx = (float)x / (float)h_res;
            float fy = (float)y / (float)v_res;
            fx += rng.random() / h_res;
            fy += rng.random() / v_res;
            dmcr::Color c = midfunc(m_scene->camera().ray(fx, fy));
            result->setPixel(x - left, y - top, c);
        }
    }
    
    return result;
}

// ye olde version:
//    // Scan the given portion of the scene
//    for (uint16_t y = top; y <= bottom; ++y) {
//        for (uint16_t x = left; x <= right; ++x) {
//            RaycastResult raycast_result = 
//                m_scene->shootRay(
//                    m_scene->camera().ray((float)x / (float)h_res,
//                                          (float)y / (float)v_res));
//            
//            /*
//             * TODO
//             * Implement me properly!
//             */
//            Color c;

//            if (raycast_result.object() == nullptr) {
//                c = { 0.0f, 0.0f, 0.0f };
//            } else {
//                if (raycast_result.object()->type() == "sphere")
//                    c = { 1.0f, 0.0f, 0.0f };
//                else
//                    c = { 0.0f, 0.0f, 1.0f };
//            }
//            result->setPixel(x - left, y - top, c);
//        }
//    }

// splitting the original function in pieces (at the moment this
// function just passes the parameter on to 'iterator')
dmcr::Color dmcr::Renderer::midfunc(dmcr::Ray ray) const
{
    return dmcr::Renderer::iterator(ray);
}

// getting pixel color
dmcr::Color dmcr::Renderer::iterator(dmcr::Ray ray) const
{
    RaycastResult raycast_result = m_scene->shootRay(ray);
    dmcr::Color c;

    if (raycast_result.object() == nullptr) {
        c = { 0.0f, 0.0f, 0.0f };
    } else {
        if (raycast_result.object()->type() == "sphere")
            c = { 1.0f, 0.0f, 0.0f };
        else
            c = { 0.0f, 0.0f, 1.0f };
    }
    return c;
}
