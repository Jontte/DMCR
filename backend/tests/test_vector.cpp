/*
 * This file is a part of the DMCR project and is subject to the terms and
 * conditions defined in file 'LICENSE.txt', which is a part of this source
 * code package.
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include "../src/vector.h"

// Macro double-expansion for proper stringify
#define S(x) #x
#define S_(x) S(x)
#define LINEREF "Assertion failed: " __FILE__ ":" S_(__LINE__)

namespace  {

const double epsilon = 0.000001; 

void assert(bool cond, const char * msg) {
    if(!cond)
        throw std::runtime_error(msg);
}
void assert_fails(bool cond, const char * msg) { assert(!cond, msg);} 

template <class T1, class T2>
void assert_double_equal(T1 t1, T2 t2, const char * msg)
{
    assert(abs(t1-t2) < epsilon, msg);
}
template <class T>
void assert_vector_equal(const dmcr::Vector3<T>& v1, const dmcr::Vector3<T>& v2, const char * msg)
{
    assert_double_equal(v1.x(), v2.x(), msg);
    assert_double_equal(v1.y(), v2.y(), msg);
    assert_double_equal(v1.z(), v2.z(), msg);
}

#define ASSERT(cond) assert(cond, LINEREF);
#define ASSERT_FAILS(cond) assert_fails(cond, LINEREF);
#define ASSERT_FLOAT_EQUAL(f1,f2) assert_double_equal((f1), (f2), LINEREF);
#define ASSERT_VECTOR_EQUAL(v1,v2) assert_vector_equal((v1), (v2), LINEREF);

}

void test_vector()
{
    // Test constructors
    {
        dmcr::Vector3f v1;
        ASSERT_FLOAT_EQUAL(v1.x(), 0);
        ASSERT_FLOAT_EQUAL(v1.y(), 0);
        ASSERT_FLOAT_EQUAL(v1.z(), 0);
        dmcr::Vector3f v2(1,2,3);
        ASSERT_FLOAT_EQUAL(v2.x(), 1);
        ASSERT_FLOAT_EQUAL(v2.y(), 2);
        ASSERT_FLOAT_EQUAL(v2.z(), 3);
    }
    // Test setters
    {
        dmcr::Vector3f v1(0,4,0);
        ASSERT_FLOAT_EQUAL(v1.x(), 0);
        ASSERT_FLOAT_EQUAL(v1.y(), 4);
        ASSERT_FLOAT_EQUAL(v1.z(), 0);

        dmcr::Vector3f v2(1,-4,5);

        v1[0] += 1;
        v1[1] -= 8;
        v1[2] += 5;

        ASSERT_VECTOR_EQUAL(v1, v2);

    }
    // Test length
    {
        dmcr::Vector3f v1(0,0,0);
        ASSERT_FLOAT_EQUAL(v1.length(), 0);
        v1.setX(64);
        ASSERT_FLOAT_EQUAL(v1.length(), 64);
        v1.normalize();
        ASSERT_FLOAT_EQUAL(v1.length(), 1);
    }
}


