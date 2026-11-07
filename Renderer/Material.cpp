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

uint32_t Material::returnColourAtUV(const int& u, const int& v)
{
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);

    if(x>width || y > height)
    {
        return RED;
    }

    return texture[width*y + x];
}
