#pragma once
#include "OBJLoader.h"
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtx/rotate_vector.hpp"
#include "threeDModel.h"
#include "Line.h"
#include "InputManager.h"
class Carriage
{
private:
	CShader* shader;
	CThreeDModel carriage;
	int pointCount;
	float Speed;
	float MinimumSpeed;
	float acceleration;
	glm::vec3 Position;
	glm::vec3 FrontOrientation;
	glm::vec3 yawAxis;
	glm::vec3 pitchAxis;
	glm::mat4 modelMatrix;
	bool moving;
	bool tempE;


public:
	Carriage(float Material_Amb[4], float Material_Dif[4], float Material_Spec[4], float Material_Shin);
	void init(COBJLoader& objLoader, glm::vec3 position, CShader* shader, std::string geometryPath);
	void render();
	void update(Line& l, double timeStep);
	
	inline glm::vec3 getCarriagePosition() { return Position; }
	inline glm::mat4 getModelMatrix() { return modelMatrix; }






};

