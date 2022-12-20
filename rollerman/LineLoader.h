#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "glm\glm.hpp"
#include "Line.h"
#include <sstream>
namespace LineLoader
{

	void loadLine(Line& line, std::string filePath);
};

