#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/transform.hpp"
#include "Sphere.h"
#include "GL\freeglut.h"
#include "InputManager.h"
#include "Octree/Octree.h"
#include "3dStruct/threeDModel.h"
namespace Camera
{
	

	void Update(double timeStep, glm::vec3 carriagePosition, glm::mat4& carriageTransform, std::vector<CThreeDModel*> collidables);
	const glm::vec3& getCameraPosition();
	const glm::vec3& getCameraTarget();
	glm::mat4 getViewingMatrix();
	bool CoctreeCollisionCheck(COctree* c, glm::vec3 position, float sphereRadius);
	bool SphereAxisAllignedCuboidCollision(glm::vec3 spherePos, float sphereRadius, glm::vec3 cuboidOriginPos, glm::vec3 cuboidSideLengths);
};

