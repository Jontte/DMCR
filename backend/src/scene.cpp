#include "scene.h"
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include "vector.h"
#include "sceneobject.h"

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

    std::string data((std::istreambuf_iterator<char>(input_file)),
                     std::istreambuf_iterator<char>());

    input_file.close();

    loadFromString(data);
}

void dmcr::Scene::loadFromString(const std::string &string)
{
    Json::Value root;
    Json::Reader reader;
    bool parsing_succesful = reader.parse(string, root);
    if (!parsing_succesful)
        throw dmcr::SceneException(std::string("Failed to parse scene") +
                                   reader.getFormattedErrorMessages());

    const Json::Value objects = root["scene"];
    for (unsigned int i = 0; i < objects.size(); ++i) {
        const Json::Value position = objects[i]["position"];
        dmcr::Vector3f position_value;

        if (!position || !position.isArray() || position.size() != 3)
            throw SceneException("No position specified for object");
            
        position_value = dmcr::Vector3f(position[0u].asDouble(),
                                        position[1].asDouble(),
                                        position[2].asDouble());

        dmcr::SceneObjectPtr object;
        
        std::string type = objects[i]["type"].asString();
        if (type == "sphere") {
            std::shared_ptr<dmcr::Sphere> sphere(new dmcr::Sphere);
            
            const Json::Value radius = objects[i]["radius"];
            float radius_value = 1.0f;
            
            if (!radius)
                throw SceneException("No radius specified for sphere");
            
            radius_value = radius.asDouble();

            sphere->setRadius(radius_value);
            object = sphere;
        } else if (type == "box") {
            std::shared_ptr<dmcr::Box> box(new dmcr::Box);

            object = box;
        } else {
            throw SceneException(std::string("Unknown object type ") + type);
        }

        object->setPosition(position_value);
        addObject(object);
    }
}
