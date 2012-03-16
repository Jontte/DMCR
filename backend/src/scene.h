/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include <list>
#include "sceneobject.h"
#include "ray.h"
#include "camera.h"
#include "unique_ptr"
#include "raycastresult.h"

namespace dmcr {

class SceneException : public std::runtime_error
{
public:
    SceneException(const std::string& msg)
        : std::runtime_error(msg) {}
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

    virtual void beginAddObjects() = 0;
    virtual void endAddObjects() = 0;

    /*!
     * \brief Add object to scene
     *
     * Adds given object to the scene. Implement this method in a
     * subclass.
     * \param object Object to add
     */
    virtual void addObject(std::unique_ptr<dmcr::SceneObject> object) = 0;
    
    /*!
     * \brief Get list of possible objects in path of given ray
     * \param ray A Ray
     * \return List of objects, which have a chance of intersecting the given ray
     *
     * This method gets a list of objects which are to be considered for ray
     * intersection. The specific list depends on the derived class's data
     * structure implementation.
     */
    virtual std::list<SceneObjectPtr> intersectionCandidates(
            const dmcr::Ray& ray) const = 0;

    /*!
     * \brief Shoot a ray in the scene
     * 
     * Shoots given ray in the scene and finds the first object it intersects.
     * Implement this method in a subclass.
     * \param ray Ray to shoot
     * \return RaycastResult object
     */
    dmcr::RaycastResult shootRay(const dmcr::Ray& ray) const;
    
    const dmcr::Camera& camera() const { return m_camera; }
    dmcr::Camera& camera() { return m_camera; }
    
protected:
    dmcr::Camera m_camera;
};

typedef std::shared_ptr<Scene> ScenePtr;

}

#endif // SCENE_H
