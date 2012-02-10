#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <memory>
#include <stdexcept>
#include "scene.h"

namespace dmcr {

struct Color {
    float r, g, b;
};

class RenderResultException : public std::runtime_error
{
public:
    RenderResultException(const std::string& msg)
        : std::runtime_error(msg) {}
};

/*!
 * \brief Stores the result of a rendering pass
 */
class RenderResult
{
public:
    RenderResult(uint16_t width, uint16_t height);
    ~RenderResult();
   
    /*!
     * \brief Set pixel to a color
     * \param x x-coordinate of pixel
     * \param y y-coordinate of pixel
     * \param color Color of pixel
     */
    void setPixel(uint16_t x, uint16_t y, Color color);
    
    /*!
     * \brief Save rendering result to PPM image
     * \throws RenderResultException
     */
    void saveImage(const std::string& file_name) const;
    
private:
    uint16_t m_width;
    uint16_t m_height;
    
    Color *m_data;
};

/*!
 * \brief Renders given scene
 */
class Renderer
{
public:
    Renderer(dmcr::ScenePtr scene) { m_scene = scene; }
    
    void setScene(dmcr::ScenePtr scene) { m_scene = scene; }
    
    /*!
     * \brief Render a given portion of the scene
     * 
     * By default the whole scene is rendered. All coordinates belong to range
     * [0, 1]. Coordinates grow from left to right and from top to bottom.
     * 
     * \param result Pointer to RenderResult into which to render
     * \param h_res Horizontal resolution of full image in pixels
     * \param v_res Vertical resolution of full image in pixels
     * \param left Left border of portion to render
     * \param right Right border of portion to render
     * \param top Top border of portion to render
     * \param bottom Bottom border of portion to render
     */
    void render(dmcr::RenderResult *result,
                uint16_t h_res, uint16_t v_res,
                float left = 0.0f, float right = 1.0f,
                float top = 0.0f, float bottom = 1.0f) const;
private:
    dmcr::ScenePtr m_scene;
};

}

#endif // RENDERER_H

