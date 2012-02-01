#ifndef VECTOR_H_INCLUDED_
#define VECTOR_H_INCLUDED_

#include <algorithm>
#include <cmath>
using std::swap;

// Scalar is just an alias for our floating point type
// This allows us to easily switch between float and double.
typedef double Scalar;

// The Vector3 class is just a simple container for three floating point numbers.
// We define accessor functions so that one can do for example the following:
/* 

Vector3 my_vector(1,2,3); // create a vector and initialize it to (1,2,3)

my_vector.x() // returns the x-component

my_vector.y() = 50; // Sets the y-component to 50

*/

class Vector3
{

// Class-private members. These are inaccessible from the outside
private: 
    Scalar x_, y_, z_;

public:
// Public members

// We define two constructors, one with and one without parameters
    Vector3(){}
    Vector3(Scalar X, Scalar Y, Scalar Z) :x_(X), y_(Y), z_(Z){}

// Const and non-const accessors to private fields
    const Scalar& x() const {return x_;}
    const Scalar& y() const {return y_;}
    const Scalar& z() const {return z_;}
    Scalar& x() {return x_;}
    Scalar& y() {return y_;}
    Scalar& z() {return z_;}
};

// Define helper functions. See vector.cpp for actual implementation
Scalar length(Vector3 v);
Scalar dot_product(Vector3 v1, Vector3 v2);


#endif


