#include "LineLoader.h"
namespace LineLoader {

	void loadLine(Line& line, std::string filePath) {

		std::string fileLine;
		std::fstream file(filePath);

		glm::vec3 point;

		while (std::getline(file, fileLine)) {
			std::stringstream parse_input(fileLine);

			std::string token;
			parse_input >> token;
			if (token == "v") {
				
				parse_input >> point.x >> point.y >> point.z;
				line.addPoint(point);
			}
			if (token == "l") {

				//std::cout << "index" << std::endl;

			}
		
		}
	}








}