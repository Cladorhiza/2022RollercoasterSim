#pragma once
#include "GL/glew.h"
#include "shaders/Shader.h"
#include <vector>
#include "glm\glm.hpp"
class Line
{
private:
	CShader* shader;
	std::vector<unsigned int> indexes;
	unsigned vbo, vao;
	std::vector<glm::vec3> points;

public:
	Line::Line();
	void init(CShader* shader);
	void Line::render();
	inline void addPoint(glm::vec3 point) {
		points.push_back(point);
	}
	inline const std::vector<glm::vec3>& getPoints() {
		return points;
	}
};

