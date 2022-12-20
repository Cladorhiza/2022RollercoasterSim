#include "Carriage.h"

Carriage::Carriage(float Material_Amb[4], float Material_Dif[4], float Material_Spec[4], float Material_Shin)
	:Position(0.f, 0.f, 0.f),
	FrontOrientation(-1.f, 0.f, 0.f),
	yawAxis(0.f, 1.f, 0.f),
	pitchAxis(0.f, 0.f, 1.f),
	pointCount(0),
	Speed(5.f),
	MinimumSpeed(2.f),
	acceleration(1.f),
	shader(0),
	modelMatrix(1.f),
	moving(true),
	tempE(false),
	carriage(Material_Amb, Material_Dif, Material_Spec, Material_Shin)
{


}

void Carriage::init(COBJLoader& objLoader, glm::vec3 position, CShader* tempShader, std::string geometryPath) {
	shader = tempShader;
	if (objLoader.LoadModel(geometryPath))//returns true if the model is loaded
	{
		carriage.ConstructModelFromOBJLoader(objLoader);
		carriage.InitVBO(shader);
	}

	Position = position;
}
void Carriage::render() {

	carriage.DrawElementsUsingVBO(shader);

}
void Carriage::update(Line& l, double timeStep) {

	if (InputManager::getKey_E() && !tempE) {
		moving = !moving;
	}
	tempE = InputManager::getKey_E();
	if (!moving) return;

	Speed = Speed + acceleration * timeStep;
	if (Speed < MinimumSpeed) Speed = MinimumSpeed;
	float distanceLeft = Speed * timeStep;
	bool moveComplete = false;
	int nextPoint = (pointCount == l.getPoints().size() - 1) ? 0 : pointCount + 1;
	glm::vec3 oldCarriagePosition(Position);
	glm::vec3 carriageNewDirection(l.getPoints()[nextPoint] - Position);
	while (!moveComplete) {


		if (distanceLeft > glm::length(carriageNewDirection)) {
			distanceLeft -= glm::length(carriageNewDirection);
			Position += carriageNewDirection;
		}
		else {
			carriageNewDirection = glm::normalize(carriageNewDirection);
			Position += carriageNewDirection * distanceLeft;
			moveComplete = true;
		}

		if (!moveComplete) {
			(pointCount == l.getPoints().size() - 1) ? pointCount = 0 : pointCount++;
			nextPoint = (pointCount == l.getPoints().size() - 1) ? 0 : pointCount + 1;
			carriageNewDirection = l.getPoints()[nextPoint] - l.getPoints()[pointCount];
		}
	}
	//translate
	modelMatrix = glm::translate(glm::mat4(1.f), Position);

	//yaw
	glm::vec3 temp(glm::normalize(glm::vec3(carriageNewDirection.x, 0.f, carriageNewDirection.z)));
	float dot = glm::dot(FrontOrientation, temp);
	if (dot < -1.f) dot = -1.f;
	if (dot > 1.f) dot = 1.f;
	glm::vec3 crossY = glm::cross(FrontOrientation, temp);
	float yaw = acosf(dot);

	//pitch
	dot = glm::dot(carriageNewDirection, temp);
	if (dot < -1.f) dot = -1.f;
	if (dot > 1.f) dot = 1.f;
	glm::vec3 crossP = glm::cross(carriageNewDirection, temp);
	float pitch = -acosf(dot);

	//check handedness
	glm::vec3 yawAxisLocal = glm::rotate(yawAxis, pitch, pitchAxis);
	if (glm::dot(crossY, yawAxisLocal) < 0)
		yaw = -yaw;

	glm::vec3 pitchAxisLocal = glm::rotate(pitchAxis, yaw, yawAxis);
	if (glm::dot(crossP, pitchAxisLocal) < 0)
		pitch = -pitch;

	//acceleration
	if (pitch > 0) acceleration = 4.5f * pitch;
	else if (pitch < 0) acceleration = 5.5f * pitch;
	else acceleration = -2.f;

	modelMatrix = glm::rotate(modelMatrix, pitch, pitchAxis);
	glm::vec3 tempYawhAxis(yawAxis);
	tempYawhAxis = glm::rotate(tempYawhAxis, -pitch, pitchAxis);
	modelMatrix = glm::rotate(modelMatrix, yaw, tempYawhAxis);
}

