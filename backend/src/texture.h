#ifndef DMCR_TEXTURE_H
#define DMCR_TEXTURE_H

#include "png++/image.hpp"
#include "png++/rgb_pixel.hpp"

#include "renderer.h"

#include <string>
#include <memory>

namespace dmcr {
    class Texture2D {
    public:
        virtual Color queryTexel(double u, double v) const = 0;
    };
    
    class ImageTexture : public Texture2D {
    public:
        ImageTexture(std::string path);
        
        virtual Color queryTexel(double u, double v) const;
        
    private:
        png::image<png::rgb_pixel> m_image;
    };

    class ColorTexture : public Texture2D {
    public:
        ColorTexture(Color color)
        : Texture2D(), m_color(color) { }

        virtual Color queryTexel(double u, double v) const {
            (void)u; (void)v;
            return m_color;
        }

    private:
        Color m_color;
    };

    typedef std::shared_ptr<Texture2D> Texture2DPtr;
}

#endif

