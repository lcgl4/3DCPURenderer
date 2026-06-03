#include "objReader.h"

void loadEntityFromFile(const std::string name, Entity& obj)
{
    std::ifstream file(name);
    std::string line, prefix;
    std::vector < Vec <float, 3> > vertices;
    std::vector < Vec <int, 3> > faces;
    std::vector <int> singleFace;
    std::vector <uint32_t> colours;

    Vec <float, 3> vertex;

        if (file.is_open()) {
            
            //vertices
            while (std::getline(file, line)) {

                std::istringstream iss(line);
                iss >> prefix;

                if (prefix =="v") {

                    iss >> vertex.x >> vertex.y >> vertex.z;
                    vertices.push_back(vertex);
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
                    }


                    for (int i = 1; i < singleFace.size() - 1; i++) {
                        int a = singleFace[0];
                        int b = singleFace[i];
                        int c = singleFace[i + 1];

                        faces.push_back({a,b,c});
                        colours.push_back(getNextColor());
                    }
                    singleFace.clear();
                }
            }

            normalizeModelCoordinates(vertices);

            obj.setVertices(vertices);
            obj.setFaces(faces);
            obj.setColours(colours);

            file.close();
        }
}

void normalizeModelCoordinates(std::vector<Vec<float, 3>>& v)
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
