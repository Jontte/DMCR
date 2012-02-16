#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <memory>
#include <stdexcept>
#include "scene.h"
#include <vector>

namespace dmcr {

/*! \brief Holds the color of one pixel in RGB as three floats */
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
    /*! \brief Construct a render result
     * 
     * \param left X coordinate of left border of rendered area in pixels
     * \param right X coordinate of right border of rendered area in pixels
     * \param top Y coordinate of top border of rendered area in pixels
     * \param bottom Y coordinate of bottom border of rendered area in pixels
     */
    RenderResult(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom);
   
    /*!
     * \brief Set pixel to a color
     * \param x x-coordinate of pixel
     * \param y y-coordinate of pixel
     * \param color Color of pixel
     */
    void setPixel(uint16_t x, uint16_t y, Color color) {
        m_data[y * m_width + x] = color;
    }

    /*!
     * \brief Save rendering result to PPM image
     * \throws RenderResultException
     */
    void saveImage(const std::string& file_name) const;
    
    uint16_t left() const { return m_left; }
    uint16_t right() const { return m_right; }
    uint16_t top() const { return m_top; }
    uint16_t bottom() const { return m_bottom; }
    uint16_t width() const { return m_width; }
    uint16_t height() const { return m_height; }
    
    /*! \brief Get rendered image as array of Color structs */
    const std::vector<Color>& data() const { return m_data; }
    
private:
    uint16_t m_left, m_right, m_top, m_bottom;
    uint16_t m_width;
    uint16_t m_height;
    
    std::vector<Color> m_data;
};

typedef std::shared_ptr<dmcr::RenderResult> RenderResultPtr;

/*!
 * \brief Renders given scene
 */
class Renderer
{
public:
    /*! \brief Construct a renderer
     * 
     * \param scene Scene object to render
     */
    Renderer(dmcr::ScenePtr scene) { m_scene = scene; }
    
    void setScene(dmcr::ScenePtr scene) { m_scene = scene; }
    
    /*!
     * \brief Render a given portion of the scene
     * 
     * By default the whole scene is rendered. 
     * 
     * \param h_res Horizontal resolution of full image in pixels
     * \param v_res Vertical resolution of full image in pixels
     * \param left Left border of portion to render in pixels
     * \param right Right border of portion to render in pixels
     * \param top Top border of portion to render in pixels
     * \param bottom Bottom border of portion to render in pixels
     * \return Pointer to RenderResult object
     */
    std::shared_ptr<dmcr::RenderResult> render(uint16_t h_res, uint16_t v_res,
                                               uint16_t left = 0, 
                                               uint16_t right = 0,
                                               uint16_t top = 0, 
                                               uint16_t bottom = 0) const;
private:
    dmcr::ScenePtr m_scene;
};

}

#endif // RENDERER_H

