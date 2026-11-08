#include "Material.h"

Material::Material(uint32_t* t, int w, int h)
{
    this->texture = t;
    this->width = w;
    this->height = h;

}

Material::~Material()
{
}

uint32_t Material::returnColourAtUV(const float& u, const float& v)
{
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);

    if(x < 0 || x >= width || y < 0 || y >= height)
    {
        return RED;
    }

    return texture[width*y + x];
}
