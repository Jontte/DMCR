#ifndef MATERIAL_H
#define MATERIAL_H

#include"vector.h"

//sets up diffuse, reflection, emit, transmit and absorption for a certain surface
//(absorption = 1 - reflection - transmit - emit)

namespace DMCR {
    
class Material
{
public:
    // Return color of material
    dmcr::Color color() const;
    

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
    double ior() const;
private:
    double m_diffuse;
    double m_specular;
    double m_transmit;
    double m_emit;
    double m_ior;
};

}

#endif // MATERIAL_H
