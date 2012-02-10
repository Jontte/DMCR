#include "renderer.h"
#include <fstream>

dmcr::RenderResult::RenderResult(uint16_t width, uint16_t height) :
    m_width(width),
    m_height(height)
{
    m_data = new Color[width * height];
}

dmcr::RenderResult::~RenderResult()
{
    delete [] m_data;
}

void dmcr::RenderResult::setPixel(uint16_t x, uint16_t y, Color c)
{
    m_data[y * m_width + x] = c;
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

void dmcr::Renderer::render(dmcr::RenderResult *result, 
                            uint16_t h_res, uint16_t v_res,
                            float left, float right,
                            float top, float bottom) const
{
    // Convert given coordinates to pixel coordinates
    uint16_t pixel_left = left * h_res;
    uint16_t pixel_right = right * h_res;
    uint16_t pixel_top = top * v_res;
    uint16_t pixel_bottom = bottom * v_res;
    
    // Scan the given portion of the scene
    for (uint16_t y = pixel_top; y <= pixel_bottom; ++y) {
        for (uint16_t x = pixel_left; x <= pixel_right; ++x) {
            /*
             * TODO
             * Implement me properly!
             */
            RaycastResult raycast_result = 
                m_scene->shootRay(m_scene->camera().ray(x, y));
                
            Color c;
            
            if (raycast_result.object()->type() == "sphere")
                c = { 1.0f, 0.0f, 0.0f };
            else
                c = { 0.0f, 0.0f, 0.0f };
            result->setPixel(x, y, c);
        }
    }
}
                                         
                                     
                                     
