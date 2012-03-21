/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <cstdint>
#include <memory>
#include <stdexcept>
#include "scene.h"
#include <vector>
#include "random.h"

namespace dmcr {

class RenderResultException : public std::runtime_error
{
public:
    RenderResultException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class RenderResult;
typedef std::shared_ptr<dmcr::RenderResult> RenderResultPtr;

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
    void setPixel(uint16_t x, uint16_t y, dmcr::Color color) {
        m_data[y * m_width + x] = color;
    }

    const dmcr::Color& pixel(uint16_t x, uint16_t y) const {
        return m_data.at(y * m_width + x);
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
    uint32_t iterationsDone() const { return m_iterations_done; }
    
    /*! \brief Get rendered image as array of Color structs */
    const std::vector<dmcr::Color>& data() const { return m_data; }

    /*! \brief Blend into a subrectangle of other render result
     * Blends using a moving average
     * \param count count of blended samples before this sample
     * THREAD UNSAFE
     */
    void blendInto(dmcr::RenderResultPtr result, uint32_t count);
    
private:
    uint16_t m_left, m_right, m_top, m_bottom;
    uint16_t m_width;
    uint16_t m_height;
    uint32_t m_iterations_done;
    
    std::vector<dmcr::Color> m_data;
};

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
     * \param h_res Horizontal resolution of full image in pixels
     * \param v_res Vertical resolution of full image in pixels
     * \param left Left border of portion to render in pixels (inclusive)
     * \param right Right border of portion to render in pixels (inclusive)
     * \param top Top border of portion to render in pixels (inclusive)
     * \param bottom Bottom border of portion to render in pixels (inclusive)
     * \return Pointer to RenderResult object
     * 
     * By default, if this function is called with only the resolution
     * parameters, the whole image is rendered.
     */
    virtual
    std::shared_ptr<dmcr::RenderResult> render(uint16_t h_res, uint16_t v_res,
                                               uint16_t left = 0, 
                                               uint16_t right = 0,
                                               uint16_t top = 0, 
                                               uint16_t bottom = 0) const;
    
    dmcr::Color midfunc(dmcr::Ray ray) const;
    
    dmcr::Color iterator(dmcr::Ray ray, int iterations = 0) const;
private:
    dmcr::ScenePtr m_scene;
    mutable dmcr::RNG m_rng;
};

}

#endif // RENDERER_H

