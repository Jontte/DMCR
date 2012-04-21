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
    
    /*
        These functions return the probability of an arriving ray converting
        into a ray of given type.
        
        diffuse + specular + transmit + emit < 1
     */
    double diffuse() const;
    double specular() const;
    double transmit() const;
    double emit() const;
    
    // Return refractive index of material
    double ior() const;
};

}

#endif // MATERIAL_H
