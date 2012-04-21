#include "texture.h"
#include <cmath>
#include <iostream>

dmcr::ImageTexture::ImageTexture(std::string path)
: Texture2D()
, m_image(path)
{
}

dmcr::Color dmcr::ImageTexture::queryTexel(double u, double v) const
{
    auto w = m_image.get_width()-1;
    auto h = m_image.get_height()-1;
    auto px = m_image.get_pixel(round(u * w), round(v * h));
    
    return dmcr::Color(px.red, px.green, px.blue);
}
