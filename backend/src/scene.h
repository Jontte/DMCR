#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <stdexcept>
#include "sceneobject.h"

namespace dmcr {

class SceneException : public std::runtime_error
{
public:
    SceneException(const std::string& msg)
        : std::runtime_error(msg) {}
};

/*!
  \brief Parses and contains the scene to be rendered

  This class is an abstract base class for a pathtracing scene.
  Subclasses must implement a data structure in which to store
  parsed objects.
  */
class Scene
{
public:
    Scene();
    /*!
      \brief Load scene from file

      Primarily for testing purposes.
      \param file_name Name of file to load
      \throws SceneException
      */
    void loadFromFile(const std::string &file_name);

    /*!
      \brief Load scene from given string

      Parses given string and adds found objects to scene
      \param string String to parse
      \throws SceneException
      */
    void loadFromString(const std::string &string);

    /*!
      \brief Add object to scene

      Adds given object to scene. Implement this method in a
      subclass.
      \param object Object to add
      */
    virtual void addObject(dmcr::SceneObjectPtr &object) = 0;
};

}

#endif // SCENE_H
