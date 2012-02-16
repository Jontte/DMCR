#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "sceneobject.h"
#include "ray.h"
#include "camera.h"
#include "unique_ptr"

namespace dmcr {

class SceneException : public std::runtime_error
{
public:
    SceneException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class RaycastResult
{
public:
    RaycastResult() { }
    
    void setObject(dmcr::SceneObjectPtr object) { m_object = object; }
    dmcr::SceneObjectPtr object() const { return m_object; }
    
    void setIntersectionPoint(const dmcr::Vector3f& intersection_point) {
        m_intersection_point = intersection_point; 
    }
    dmcr::Vector3f intersectionPoint() const { return m_intersection_point; }
    
private:
    dmcr::SceneObjectPtr m_object;
    dmcr::Vector3f m_intersection_point;
};

/*!
 * \brief Parses and contains the scene to be rendered
 *
 * This class is an abstract base class for a pathtracing scene.
 * Subclasses must implement a data structure in which to store
 * parsed objects.
 */
class Scene
{
public:
    Scene();
    /*!
     * \brief Load scene from file
     *
     * Primarily for testing purposes.
     * \param file_name Name of file to load
     * \throws SceneException
     */
    void loadFromFile(const std::string &file_name);

    /*!
     * \brief Load scene from given string
     *
     * Parses given string and adds found objects to scene
     * \param string String to parse
     * \throws SceneException
     */
    void loadFromString(const std::string &string);

    /*!
     * \brief Add object to scene
     *
     * Adds given object to the scene. Implement this method in a
     * subclass.
     * \param object Object to add
     */
    virtual void addObject(std::unique_ptr<dmcr::SceneObject> object) = 0;
    
    /*!
     * \brief Shoot a ray in the scene
     * 
     * Shoots given ray in the scene and finds the first object it intersects.
     * Implement this method in a subclass.
     * \param ray Ray to shoot
     * \return RaycastResult object
     */
    virtual dmcr::RaycastResult shootRay(const dmcr::Ray& ray) const = 0;
    
    dmcr::Camera camera() const { return m_camera; }
    
protected:
    dmcr::Camera m_camera;
};

typedef std::shared_ptr<Scene> ScenePtr;

}

#endif // SCENE_H
