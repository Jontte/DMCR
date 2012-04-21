#ifndef DMCR_TEXTURE_H
#define DMCR_TEXTURE_H

#include "png++/image.hpp"
#include "png++/rgb_pixel.hpp"

#include "renderer.h"

#include <string>

namespace dmcr {
    class Texture2D {
    public:
        virtual Color queryTexel(double u, double v) = 0;
    };
    
    class ImageTexture : public Texture2D {
    public:
        ImageTexture(std::string path);
        
        virtual Color queryTexel(double u, double v);
        
    private:
        png::image<png::rgb_pixel> m_image;
    };
}

#endif
