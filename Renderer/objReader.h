#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "Entity.h"
#include <algorithm>

void loadEntityFromFile(const std::string name, Entity& obj);

void normalizeModelCoordinates(std::vector < Vec <float, 3> >& vertices);