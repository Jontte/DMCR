/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef DMCR_CAMERA_H
#define DMCR_CAMERA_H

#include "vector.h"
#include "ray.h"

namespace dmcr {

/*!
 * \brief Describes a camera in a scene
 *
 * The Camera class converts screen space coordinates into 3D rays. The camera
 * is characterized by it's position, look-at point, field of view and aspect
 * ratio. The workhorse of the Camera class is the function \ref ray which deals
 * with the actual conversion.
 */
class Camera
{
public:
    Camera() : m_fov(0), m_aspect(0) { }
    
    /*!
     * \brief Calculate a ray corresponding to the given pixel
     * \param x Pixel x-coordinate in range [0, 1]
     * \param y Pixel y-coordinate in range [0, 1]
     * \return A ray from the camera's position to the given pixel
     *
     * This function calculates and returns a ray corresponding to a point in
     * the camera's viewing plane. The coordinates are in the range [0, 1], 
     * where the point (0, 0) corresponds to the top-left corner of the screen.
     */
    dmcr::Ray ray(float x, float y) const;
    
    /*!
     * \brief Set camera position
     * \param position New position
     */
    void setPosition(const dmcr::Vector3f& position) { m_position = position; }
    /*!
     * \brief Get camera position
     * \return Camera's position
     */
    dmcr::Vector3f position() const { return m_position; }
    
    /*!
     * \brief Set point what camera is looking at
     * \param look_at Point to look at
     *
     * This function sets the point what the camera is looking at. The point
     * is given in reference to the scene origin.
     */
    void setLookAt(const dmcr::Vector3f& look_at) { m_look_at = look_at; }
    /*!
     * \brief Get point what camera is looking at
     * \return Point what camera is looking at
     */
    dmcr::Vector3f lookAt() const { return m_look_at; }
    
    /*!
     * \brief Set horizontal field of view of camera
     * \param fov Horizontal field of view of camera
     *
     * The horizontal field of view of the camera is defined as the angle
     * between the leftmost and rightmost visible pixels from the camera's
     * view.
     */
    void setFov(float fov) { m_fov = fov; }
    /*!
     * \brief Get horizontal field of view of camera
     * \return Camera's horizontal field of view
     */
    float fov() const { return m_fov; }
    
    /*!
     * \brief Set aspect ratio of camera
     * \param aspect Aspect ratio of camera
     *
     * The aspect ratio of the camera is the width of the camera's view divided 
     * by the height of the camera's view.
     */
    void setAspect(float aspect) { m_aspect = aspect; }
    /*!
     * \brief Get aspect ratio of camera
     * \return Aspect ratio of camera
     */
    float aspect() const { return m_aspect; }
    
private:
    dmcr::Vector3f m_position;
    dmcr::Vector3f m_look_at;
    float m_fov;
    float m_aspect;
};

}

#endif // DMCR_CAMERA_H
