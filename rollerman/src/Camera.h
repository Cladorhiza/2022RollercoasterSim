#pragma once
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"
#include "gtc\matrix_inverse.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/transform.hpp"
#include "Sphere.h"
#include "freeglut.h"
#include "InputManager.h"
#include "Octree.h"
#include "threeDModel.h"
namespace Camera
{
	

	void Update(double timeStep, glm::vec3 carriagePosition, glm::mat4& carriageTransform, std::vector<CThreeDModel*> collidables);
	const glm::vec3& getCameraPosition();
	const glm::vec3& getCameraTarget();
	glm::mat4 getViewingMatrix();
	bool CoctreeCollisionCheck(COctree* c, glm::vec3 position, float sphereRadius);
	bool SphereAxisAllignedCuboidCollision(glm::vec3 spherePos, float sphereRadius, glm::vec3 cuboidOriginPos, glm::vec3 cuboidSideLengths);
};

