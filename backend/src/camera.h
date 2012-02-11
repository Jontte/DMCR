#ifndef DMCR_CAMERA_H
#define DMCR_CAMERA_H

#include "vector.h"
#include "ray.h"

namespace dmcr {

class Camera
{
public:
    Camera() : m_fov(0), m_aspect(0) { }
    
    /*!
     * \brief Calculate a ray corresponding to the given pixel
     * \param x Pixel x-coordinate
     * \param y Pixel y-coordinate
     * \return A ray from the camera's position to the given pixel
     */
    dmcr::Ray ray(float x, float y) const;
    
    void setPosition(const dmcr::Vector3f& position) {m_position = position; }
    dmcr::Vector3f position() const { return m_position; }
    
    void setLookAt(const dmcr::Vector3f& look_at) { m_look_at = look_at; }
    dmcr::Vector3f lookAt() const { return m_look_at; }
    
    void setFov(float fov) { m_fov = fov; }
    float fov() const { return m_fov; }
    
    void setAspect(float aspect) { m_aspect = aspect; }
    float aspect() const { return m_aspect; }
    
private:
    dmcr::Vector3f m_position;
    dmcr::Vector3f m_look_at;
    float m_fov;
    float m_aspect;
};

}

#endif // DMCR_CAMERA_H
