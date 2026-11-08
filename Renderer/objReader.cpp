#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "objReader.h"

uint32_t getNextColor()//for getting default no texture colours
{
    static int c = -1;
    c++;
    if (c >= NUMBER_OF_COLORS - 1) {// -1 bc not taking black into account
        c = 0;
    }

    switch (c) {
    case 0:
        return GREEN;
        break;
    case 1:
        return BLUE;
        break;
    case 2:
        return DBLUE;
        break;
    case 3:
        return PURPLE;
        break;
    case 4:
        return PINK;
        break;
    case 5:
        return RED;
        break;
    case 6:
        return YELLOW;
        break;
    case 7:
        return ORANGE;
        break;
    default:
        return GREEN;
        break;
    }

}


void loadEntityFromFile(const std::string modelname, Entity& obj)
{
    std::string name = modelname + ".obj";
    std::ifstream file(name);
    std::string line, prefix;

    std::vector < Vec <float, 3> > vertices;
    std::vector < Vec <float, 2> > vts;
    std::vector < Vec <int, 3> > faces;
    std::vector < Vec <int, 3> > vtfaces;
    std::vector <int> singleFace;
    std::vector <int> singleFaceVt;

    Vec <float, 3> vertex;
    Vec <float, 2> vt;

        if (file.is_open()) {
            
            //vertices
            while (std::getline(file, line)) {

                std::istringstream iss(line);
                iss >> prefix;

                if (prefix =="v") {

                    iss >> vertex.x >> vertex.y >> vertex.z;
                    vertices.push_back(vertex);
                }
                if (prefix =="vt") {

                    iss >> vt.x >> vt.y;
                    vts.push_back(vt);
                }

                //faces
                if (prefix=="f") {
                    
                    std::string index;
                    while (iss >> index) {
                        int i = 0;
                        int faceVert = 0;

                        while (i < index.size() && index[i] != '/')
                        {
                            faceVert = faceVert * 10 + (index[i] - '0');
                            i++;
                        }
                        faceVert--;
                        singleFace.push_back(faceVert);

                        if (i < index.size() && index[i] == '/') {
                            i++;
                        }

                        faceVert = 0;
                        while (i < index.size() && index[i] != '/')
                        {
                            faceVert = faceVert * 10 + (index[i] - '0');
                            i++;
                        }
                        faceVert--;
                        singleFaceVt.push_back(faceVert);
                        
                    }


                    for (int i = 1; i < singleFace.size() - 1; i++) {
                        int a = singleFace[0];
                        int b = singleFace[i];
                        int c = singleFace[i + 1];

                        int d = singleFaceVt[0];
                        int e = singleFaceVt[i];
                        int f= singleFaceVt[i + 1];

                        faces.push_back({a,b,c});
                        vtfaces.push_back({d,e,f});
                    }
                    singleFace.clear();
                    singleFaceVt.clear();
                }
            }

            normalizeModelCoordinates(vertices);

            obj.setMesh(faces, vertices, vtfaces, vts);

            file.close();
        }
}

Material* loadMaterialFromFile(const std::string modelname)
{
    std::string name = modelname + ".mtl";
    std::ifstream file(name);
    std::string line, prefix;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            iss >> prefix;

            if (prefix == "map_Kd") {
                std::string textureFilename;
                iss >> textureFilename;

                int width, height, channels;

                unsigned char* imgData = stbi_load(textureFilename.c_str(), &width, &height, &channels, 4);

                if (imgData) {
                    uint32_t* texturePixels = new uint32_t[width * height];

                    for (int i = 0; i < width * height; ++i) {
                        unsigned char r = imgData[i * 4 + 0];
                        unsigned char g = imgData[i * 4 + 1];
                        unsigned char b = imgData[i * 4 + 2];
                        unsigned char a = imgData[i * 4 + 3];

                        //Format 0xAARRGGBB
                        texturePixels[i] = ((uint32_t)a << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
                    }

                    stbi_image_free(imgData);
                    file.close();

                    return new Material(texturePixels, width, height);
                }
            }
        }
        file.close();
    }

    return nullptr;
}

void normalizeModelCoordinates(std::vector<Vec <float, 3>>& v)
{
    if (v.size() < 2)
        return;

    float xMax = v[0].x, xMin = v[0].x;
    float yMax = v[0].y, yMin = v[0].y;
    float zMax = v[0].z, zMin = v[0].z;

    for (int i = 1;i<v.size();i++) {
        xMax = std::max(v[i].x, xMax);
        xMin = std::min( v[i].x, xMin);

        yMax = std::max(v[i].y, yMax);
        yMin = std::min( v[i].y, yMin);

        zMax = std::max(v[i].z, zMax);
        zMin = std::min( v[i].z, zMin);

    }

    float sx = xMax - xMin;
    float sy = yMax - yMin;
    float sz = zMax - zMin;

    for (int i = 0; i < v.size(); i++) {
        float x = 2 * (v[i].x - xMin) / sx - 1;
        float y = 2 * (v[i].y - yMin) / sy - 1;
        float z = 2 * (v[i].z - zMin) / sz - 1;
        v[i] = {x, y, z};
    }
}
