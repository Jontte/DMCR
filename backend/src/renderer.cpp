#include "renderer.h"
#include <fstream>
#include <iostream>

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
    
    // Scan the given portion of the scene
    for (uint16_t y = top; y <= bottom; ++y) {
        for (uint16_t x = left; x <= right; ++x) {
            RaycastResult raycast_result = 
                m_scene->shootRay(
                    m_scene->camera().ray((float)x / (float)h_res,
                                          (float)y / (float)v_res));
            
            /*
             * TODO
             * Implement me properly!
             */
            Color c;

            if (raycast_result.object() == nullptr) {
                c = { 0.0f, 0.0f, 0.0f };
            } else {
                if (raycast_result.object()->type() == "sphere")
                    c = { 1.0f, 0.0f, 0.0f };
                else
                    c = { 0.0f, 0.0f, 1.0f };
            }
            result->setPixel(x - left, y - top, c);
        }
    }
    
    return result;
}
                                         
                                     
                                     
