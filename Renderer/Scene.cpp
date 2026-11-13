#include "Scene.h"

Scene::Scene()
{
    lightDir = { 0.666f, 0.666f, 0.666f };
}

Scene::~Scene()
{
}

void Scene::loadSceneFromFile(const std::string name)
{
    
    std::ifstream file(name);

    if (!file.is_open()) {
        std::cerr << "error, can't open scene file" << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line)) {

        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name;


        ss >> name;
        Entity* entity = new Entity(loadMaterialFromFile(name));

        Vec<float, 3> position;

        while(ss >> position.x >> position.y >> position.z)
        {
            entity->addInstance(position);
        }

        loadEntityFromFile(name, *entity);
        objects.push_back(entity);
    }

    file.close();
}

Entity& Scene::getSceneObjectByIndex(const int i)
{
    return *objects[i];
}

Camera& Scene::getSceneCamera()
{
    return camera;
}

void Scene::rotateLightDir(const float x, const float z)
{
    Vec<float, 3> xAxis = { 1.0f, 0.0f, 0.0f };
    Vec<float, 3> zAxis = { 0.0f, 0.0f, 1.0f };

    Quaternion rotQuat = axisAngle(xAxis, x);
    lightDir = rotateVector(rotQuat, lightDir);

    rotQuat = axisAngle(zAxis, z);
    lightDir = rotateVector(rotQuat, lightDir);

    normalizeNormal(lightDir);
}

const Vec<float, 3>& Scene::getLightDir()
{
    return lightDir;
}

int Scene::getNumberOfObjectsContained()
{
    return objects.size();
}
