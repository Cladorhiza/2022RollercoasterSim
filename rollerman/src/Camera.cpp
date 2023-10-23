#include "Camera.h"
#include <iostream>
namespace Camera {

	glm::mat4 cameraRotation(1.f);
	glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f); //vector for the position of the camera.
	glm::vec3 cameraX(1.f, 0.f, 0.f); //camera relative X axis
	glm::vec3 cameraY(0.f, 1.f, 0.f); //camera relative Y axis
	glm::vec3 cameraTarget(1.f);
	glm::vec3 groundedPosition(0.f, 1.5f, 0.f);
	glm::vec3 freePosition(0.f, 1.5f, 0.f);
	

	//0 = grounded, 1 = immobilized, 2 = free
	int cameraMode = 1;
	bool tempQ;

	float camSpeed = 0;
	float camStrafeSpeed = 0;
	float spinX = 0.0f, spinY = 0.0f;
	float radius = 0.5f;

	const glm::vec3& getCameraPosition() { return cameraPosition; }
	const glm::vec3& getCameraTarget() { return cameraTarget; }

	glm::mat4 getViewingMatrix() {
		return glm::lookAt(Camera::getCameraPosition(), Camera::getCameraTarget(), cameraY);
	}
	void Update(double timeStep, glm::vec3 carriagePosition, glm::mat4& carriageTransform, std::vector<CThreeDModel*> collidables) {
		if (InputManager::getKey_Q() && !tempQ) {
			cameraMode = (cameraMode + 1) % 3;
		}
		tempQ = InputManager::getKey_Q();

		if (cameraMode == 0) {
			
			cameraPosition = carriageTransform * glm::vec4(0.6f, 1.2f, 0.f, 1.f);
			cameraTarget = carriageTransform * glm::vec4(-3.f, 1.2f, 0.f, 1.f);

		}


		if (cameraMode == 1) {

			cameraPosition = groundedPosition;
			cameraTarget = carriagePosition;
		}


		if (cameraMode == 2) {
		
			float xTrans = 0.f, yTrans = 0.f, zTrans = 0.f;
			float transSpeed = 5.f;
			camSpeed = 0;
			camStrafeSpeed = 0;
			if (InputManager::getKey_LeftArrow())
			{
				spinY += 1.0f * timeStep;
			}
			if (InputManager::getKey_RightArrow())
			{
				spinY -= 1.0f * timeStep;
			}
			if (InputManager::getKey_UpArrow())
			{
				if (spinX > -1.5f)
					spinX -= 1.0f * timeStep;
			}
			if (InputManager::getKey_DownArrow())
			{
				if (spinX < 1.5f)
					spinX += 1.0f * timeStep;
			}
			if (InputManager::getKey_A()) {
				camStrafeSpeed = +transSpeed * timeStep;
			}
			if (InputManager::getKey_D()) {
				camStrafeSpeed = -transSpeed * timeStep;
			}
			if (InputManager::getKey_W()) {
				camSpeed = transSpeed * timeStep;
			}
			if (InputManager::getKey_S()) {
				camSpeed = -transSpeed * timeStep;
			}

			cameraRotation = glm::rotate(glm::mat4(1.f), spinX, cameraX);
			cameraRotation = glm::rotate(cameraRotation, spinY, cameraY);

			cameraX = glm::rotate(glm::vec3(1.f, 0.f, 0.f), spinY, cameraY);
			
			
			freePosition.x += cameraRotation[2][0] * camSpeed;
			freePosition.y += cameraRotation[2][1] * camSpeed;
			freePosition.z += cameraRotation[2][2] * camSpeed;

			freePosition.x += cameraRotation[0][0] * camStrafeSpeed;
			freePosition.y += cameraRotation[0][1] * camStrafeSpeed;
			freePosition.z += cameraRotation[0][2] * camStrafeSpeed;
			
			bool collides = false;
			for (CThreeDModel* c : collidables) {
				if (Camera::CoctreeCollisionCheck(c->GetCOCTREE(), freePosition, radius)) {
					collides = true;
					break;
				}
			}
			if (collides) {

				freePosition.x -= cameraRotation[2][0] * camSpeed;
				freePosition.y -= cameraRotation[2][1] * camSpeed;
				freePosition.z -= cameraRotation[2][2] * camSpeed;
							   
				freePosition.x -= cameraRotation[0][0] * camStrafeSpeed;
				freePosition.y -= cameraRotation[0][1] * camStrafeSpeed;
				freePosition.z -= cameraRotation[0][2] * camStrafeSpeed;

			}


			cameraPosition = freePosition;
			cameraTarget = glm::vec3(cameraPosition.x + cameraRotation[2][0], cameraPosition.y + cameraRotation[2][1], cameraPosition.z + cameraRotation[2][2]);
		}

	}

	bool CoctreeCollisionCheck(COctree* c, glm::vec3 position, float sphereRadius) {

		std::vector<double> boundybox = c->GetBoundyBox();
		glm::vec3 boundyBoxOrigin((boundybox[3] + boundybox[0]) / 2, (boundybox[4] + boundybox[1]) / 2, (boundybox[5] + boundybox[2]) / 2);
		glm::vec3 boundyBoxLengths(boundybox[3] - boundybox[0], boundybox[4] - boundybox[1], boundybox[5] - boundybox[2]);
		//collision check
		if (SphereAxisAllignedCuboidCollision(position, sphereRadius, boundyBoxOrigin, boundyBoxLengths)) {

			if (c->GetDepth() < MAX_DEPTH) {

				for (int i = 0; i < NUM_OF_OCTREE_CHILDREN; i++) {

					COctree** children = c->GetChildren();

					if (children[i]) {
						if (CoctreeCollisionCheck(children[i], position, sphereRadius)) {
							return true;
						}
					}
				}
			}
			else {
				return true;
			}
		}
		return false;
	}

	bool SphereAxisAllignedCuboidCollision(glm::vec3 spherePos, float sphereRadius, glm::vec3 cuboidOriginPos, glm::vec3 cuboidSideLengths) {
		spherePos += 0.5f;

		glm::vec3 positiveComparisonPoint(cuboidOriginPos + (cuboidSideLengths / 2.f));
		glm::vec3 negativeComparisonPoint(cuboidOriginPos - (cuboidSideLengths / 2.f));

		glm::vec3 positivePointToSphere(spherePos - positiveComparisonPoint);
		glm::vec3 negativePointToSphere(spherePos - negativeComparisonPoint);


		if (positivePointToSphere.x > sphereRadius)return false;
		if (positivePointToSphere.y > sphereRadius)return false;
		if (positivePointToSphere.z > sphereRadius)return false;

		if (negativePointToSphere.x <= sphereRadius)return false;
		if (negativePointToSphere.y <= sphereRadius)return false;
		if (negativePointToSphere.z <= sphereRadius)return false;

		return true;
	}

}