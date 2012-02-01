#include "vector.h"

Scalar length(Vector3 v)
{
    return sqrt(
        v.x()*v.x() +
        v.y()*v.y() +
        v.z()*v.z()
    );
}
Scalar dot_product(Vector3 v1, Vector3 v2)
{
    return 
        v1.x()*v2.x() + 
        v1.y()*v2.y() +
        v1.z()*v2.z();
}



