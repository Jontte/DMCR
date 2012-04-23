/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <fstream>
#include "util.h"
#include "renderer.h"

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
    file << "P3 " << m_width << " " << m_height << " " << 1023 << "\n";

    // Write pixel array
    for (int i = 0; i < m_width * m_height; ++i) {
        // Convert doubles to chars
        std::string px;
        if (m_data[i].r() > 1.0)
            px += "1023";
        else
            px += dmcr::lexical_cast<std::string>((uint16_t)(m_data[i].r() *
            1023));
        px += " ";
        if (m_data[i].g() > 1.0)
            px += "1023";
        else
            px += dmcr::lexical_cast<std::string>((uint16_t)(m_data[i].g() *
                1023));        
        px += " ";
        if (m_data[i].b() > 1.0)
            px += "1023";
        else
            px += dmcr::lexical_cast<std::string>((uint16_t)(m_data[i].b() *
                1023));        
        px += "\n";

        file << px;
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

    double fc = (double)count;
    for (uint16_t y = top(); y <= bottom(); ++y) {
        for (uint16_t x = left(); x <= right(); ++x) {
            const auto& c = pixel(x-left(), y-top());
            const auto& r = result->pixel(x, y);
            double _r = ((r.r() * fc) + c.r()) / (fc + 1.0);
            double _g = ((r.g() * fc) + c.g()) / (fc + 1.0);
            double _b = ((r.b() * fc) + c.b()) / (fc + 1.0);
            result->setPixel(x, y, dmcr::Color{_r, _g, _b});
        }
    }
}

dmcr::RenderResultPtr dmcr::Renderer::render(uint16_t h_res, uint16_t v_res, 
                                             uint16_t left, uint16_t right, 
                                             uint16_t top, uint16_t bottom) 
const
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
            double fx = (double)x / (double)h_res;
            double fy = (double)y / (double)v_res;
            fx += m_rng.random() / h_res;
            fy += m_rng.random() / v_res;
            //std::cout << x << ", " << y << std::endl;
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
dmcr::Color dmcr::Renderer::iterator(dmcr::Ray ray, 
                                     int iterations, 
                                     dmcr::Color weight) const
{       
    if (iterations == 20 || weight.length() < m_rng.random() * 0.05)
        return { 0.0, 0.0, 0.0 };
    RaycastResult rr = m_scene->shootRay(ray);
    
    SceneObjectPtr obj = rr.object();

    if (obj != nullptr) {
        double random = m_rng.random();
        double cumul = 0.0;
        dmcr::Vector3f dir;
        double weight_factor = 0.0;
        dmcr::Vector3f color = obj->material().texture()->
            queryTexel(rr.uv().x(), rr.uv().y());        
        
        if (random <= (cumul += obj->material().diffuse())) {
            dir = m_rng.random_vector();
            if (dir.dot(rr.normal()) < 0.0)
                dir = -dir;
            weight_factor = obj->material().diffuse();
        } else if (random <= (cumul += obj->material().specular())) {
            dir = ray.direction() - 
                2 * rr.normal().dot(ray.direction()) * rr.normal();
            weight_factor = obj->material().specular();
        } else if (random <= (cumul += obj->material().transmit())) {
            double ndotl = rr.normal().dot(ray.direction());

            double nl, ne;
            if (ndotl > 0.0) {
                nl = 1.0;
                ne = obj->material().ior();
            } else {
                ne = 1.0;
                nl = obj->material().ior();
            }

            dir = (ne/nl*ndotl-sqrt(1-ne*ne/(nl*nl)*
                  (1-ndotl*ndotl)))*rr.normal()-ne/nl*ray.direction();
            weight_factor = obj->material().transmit();
        } else if (random <= (cumul += obj->material().emit())) {
            return color;
        } else {
            return { 0.0, 0.0, 0.0 };
        }
               
        return color * iterator(dmcr::Ray(rr.intersectionPoint(), dir), 
                            iterations + 1,
                            weight * color * weight_factor);
    }

    return { 0.0, 0.0, 0.0 };
}
