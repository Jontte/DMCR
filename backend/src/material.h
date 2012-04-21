#ifndef MATERIAL_H
#define MATERIAL_H

#include "vector.h"
#include "texture.h"

//sets up diffuse, reflection, emit, transmit and absorption for a certain surface
//(absorption = 1 - reflection - transmit - emit)
// additionally stores the color and index of refraction of a material

namespace dmcr {
    
class Material
{
public:
    // Initializes the double-values to 0
    Material() : m_diffuse(), m_specular(), m_transmit(), m_emit(), m_ior() { }

    // Sets values for the reflection-parameters
    void setDiffuse(double diffuse){ m_diffuse = diffuse; }
    void setSpecular(double specular){ m_specular = specular; }
    void setTransmit(double transmit){ m_transmit = transmit; }
    void setEmit(double emit){ m_emit = emit; }
    /*
        These functions return the probability of an arriving ray converting
        into a ray of given type.
        
        diffuse + specular + transmit + emit < 1
     */

    double diffuse() const{ return m_diffuse; }
    double specular() const{ return m_specular; }
    double transmit() const{ return m_transmit; }
    double emit() const{ return m_emit; }
    
    // Set and return refractive index of material
    void setIor(double ior){ m_ior = ior};
    double ior() const { return m_ior; };

    // Set and return texture of material
    void setTexture(dmcr::Texture2DPtr texture) { m_texture = texture; }
    dmcr::Texture2DPtr texture() const { return m_texture; }

private:
    double m_diffuse;
    double m_specular;
    double m_transmit;
    double m_emit;
    double m_ior;
    dmcr::Color m_color;
    dmcr::Texture2DPtr m_texture;
};

}

#endif // MATERIAL_H
