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

    int i = 0;
    while (std::getline(file, line)) {

        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name;
        Vec<float, 3> position;

        ss >> name;
        ss >> position.x;
        ss >> position.y;
        ss >> position.z;

        objects.push_back(new Entity(position));
        loadEntityFromFile(name, *objects[i]);
        i++;
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

int Scene::getNumberOfObjectsContained()
{
    return objects.size();
}
