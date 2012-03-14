#ifndef UTIL_H_
#define UTIL_H_

#include <sstream>

namespace dmcr
{

struct invalid_lexical_cast : public std::runtime_error
{
    invalid_lexical_cast() : 
        std::runtime_error("invalid lexical cast") {}
};

template <class Dest, class Src>
Dest lexical_cast(const Src& src)
{
    std::stringstream ss;
    Dest dest;
    ss << src;
    ss >> dest;
    if(!ss)
        throw invalid_lexical_cast();
    return dest;
}


}


#endif


