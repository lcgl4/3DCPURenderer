#include "Scene.h"

Scene::Scene()
{
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

const Vec<float, 3>& Scene::getLightDir()
{
    return lightDir;
}

int Scene::getNumberOfObjectsContained()
{
    return objects.size();
}
