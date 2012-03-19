/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include "scene.h"
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "vector.h"
#include "sceneobject.h"
#include "box.h"
#include "sphere.h"
#include "unique_ptr"

dmcr::Scene::Scene()
{
}

void dmcr::Scene::loadFromFile(const std::string &file_name)
{
    std::ifstream input_file;
    input_file.open(file_name.c_str());
    if (!input_file.is_open())
        throw dmcr::SceneException(std::string("Failed to open file ") +
                                   file_name);

    // Load contents of file to string data using iterators
    std::string data((std::istreambuf_iterator<char>(input_file)),
                     std::istreambuf_iterator<char>());

    input_file.close();

    loadFromString(data);
}

/* unique_ptr's are a way of maintaining strict pointer ownership.
 * They must always be constructed with a pointer and when they go out of scope
 * the pointer is always deleted. However, the owner of the pointer
 * (i.e. the controlling scope) can be transferred using std::move().
 * In this code buildObjectFromValue() allocates an object and wraps it in a
 * unique_ptr. It then returns it to loadFromString passing the ownership
 * of the pointer using std::move (leaving it out would cause a compilation
 * error). loadFromString then operates on the object and passes on the owner-
 * ship to the Scene subclass which can do anything it wants with it. The owner
 * can pass the raw 'plain old' pointer to other objects that merely need to
 * use it but the lifetime of whose is no longer than the owner's.
 * 
 * Using this system is fast (faster than using traditional pointers or
 * references!) and quite error-proof.. There can be no point during which a 
 * thrown exception can cause a memory leak of the pointer contained in the 
 * unique_ptr.
 */

static std::unique_ptr<dmcr::SceneObject> buildObjectFromValue(
    const Json::Value& value)
{
    std::string type = value["type"].asString();
    if (type == "sphere") {
        auto sphere = dmcr::make_unique<dmcr::Sphere>();
        
        const Json::Value radius = value["radius"];
        
        if (!radius || !radius.isNumeric())
            throw dmcr::SceneException("No radius specified for sphere");
        
        float radius_value = radius.asDouble();

        sphere->setRadius(radius_value);
        return std::move(sphere);
    } else if (type == "box") {
        auto box = dmcr::make_unique<dmcr::Box>();

        const Json::Value extents = value["extents"];

        if (!extents || !extents.isArray() || extents.size() != 3)
            throw dmcr::SceneException("No extents specified for box");

        box->setExtents(dmcr::Vector3f(extents[0u].asDouble(),
                                       extents[1].asDouble(),
                                       extents[2].asDouble()));
        return std::move(box);
    } else {
        throw dmcr::SceneException(std::string("Unknown object type ") + type);
    }
}

void dmcr::Scene::loadFromString(const std::string &string)
{
    Json::Value root;
    Json::Reader reader;
    bool parsing_succesful = reader.parse(string, root);
    if (!parsing_succesful)
        throw dmcr::SceneException(std::string("Failed to parse scene") +
                                   reader.getFormattedErrorMessages());

    const Json::Value camera = root["camera"];
    if (!camera)
        throw SceneException("No camera specified in scene");
    const Json::Value camera_pos = camera["position"];
    if (!camera_pos || !camera_pos.isArray() || camera_pos.size() != 3)
        throw SceneException("No position specified for camera");
    const Json::Value camera_look_at = camera["look_at"];
    if (!camera_look_at || !camera_look_at.isArray() || 
        camera_look_at.size() != 3)
        throw SceneException("No look_at specified for camera");
    const Json::Value camera_fov = camera["fov"];
    if (!camera_fov || !camera_fov.isNumeric())
        throw SceneException("No fov specified for camera");
    const Json::Value camera_aspect = camera["aspect"];
    if (!camera_aspect || !camera_aspect.isNumeric())
        throw SceneException("No aspect specified for camera");
    
    m_camera.setPosition(dmcr::Vector3f(camera_pos[0u].asDouble(),
                                        camera_pos[1].asDouble(),
                                        camera_pos[2].asDouble()));
    m_camera.setLookAt(dmcr::Vector3f(camera_look_at[0u].asDouble(),
                                      camera_look_at[1].asDouble(),
                                      camera_look_at[2].asDouble()));
    m_camera.setFov(camera_fov.asDouble());
    m_camera.setAspect(camera_aspect.asDouble());

    beginAddObjects();
        
    const Json::Value objects = root["scene"];
    for (const Json::Value& value : objects) {
        const Json::Value position = value["position"];

        if (!position || !position.isArray() || position.size() != 3)
            throw SceneException("No position specified for object");
        
        dmcr::Vector3f position_value(position[0].asDouble(),
                                      position[1].asDouble(),
                                      position[2].asDouble());
        
        const Json::Value colors = value["color"];
        
        if(!colors || !colors.isArray() || colors.size() != 3)
            throw SceneException("No color specified for object");

        dmcr::Color color;
        color.r = colors[0].asFloat();
        color.g = colors[1].asFloat();
        color.b = colors[2].asFloat();
        
        std::unique_ptr<dmcr::SceneObject> object = buildObjectFromValue(value);
        object->setPosition(position_value);
        object->setColor(color);
        
        addObject(std::move(object));
    }

    endAddObjects();
}

dmcr::RaycastResult dmcr::Scene::shootRay(const dmcr::Ray& ray) const
{
    std::list<SceneObjectPtr> objects = intersectionCandidates(ray);
    double min = 1.0 / 0.0;
    SceneObjectPtr min_obj = nullptr;

    for (auto obj : objects) {
        double d = obj->intersects(ray);
        if (d > 0.0 && d < min) {
            min_obj = obj;
            min = d;
        }
    }

    RaycastResult result;
    if (min_obj) {
        result.setObject(min_obj);
        result.setIntersectionPoint(ray.origin() + ray.direction() * min);
    }

    return result;
}
