#include "Fireworks.h"
#include "PhysicsUtils.h"

#include<glm/glm.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

std::vector<glm::vec3> GenerateOutwardFacingVectorsCircle(glm::vec3 startPoint, float radius, float angleIncrement, float yOffset)
{
	std::vector<glm::vec3> vectors;
	float maxAngle = M_PI * 2;
	float radiansIncrement = (angleIncrement * (M_PI / 180.0f));
	for (float angle = 0.0f; angle <= maxAngle; angle += radiansIncrement)
	{
		glm::vec3 newPos(startPoint.x + (cos(angle) * radius), startPoint.y + yOffset, startPoint.z + (sin(angle) * radius)); // Generate a new point away from the center using the current angle
		glm::vec3 direction = startPoint - newPos; // Get the direction using the start and new point
		glm::normalize(direction);
		vectors.push_back(direction);
	}

	return vectors;
}

ConeFirework::ConeFirework(glm::vec3 position)
	: Firework(1.0f, position)
{

}

std::vector<Firework*> ConeFirework::BeginNextStage()
{
	std::vector<Firework*> vec;

	glm::vec3 position = this->GetPosition();
	float baseY = position.y;
	float yRange = 20;
	float yIncrement = 5;
	float minY = baseY - yRange;
	float maxY = baseY + yRange;

	float baseRadius = 30.0f;
	float baseAngleIncrement = 15.0f;

	for (float y = -20.0f; y <= 20.0f; y += yIncrement)
	{
		float radiusMultiplier = ((100.0f - (abs(baseY - y) * 1.5f)) / 100.0f);
		float angleMultiplier = (100.0f - (abs(baseY - y) * 0.5f)) / 100.0f;
		for (glm::vec3 direction : GenerateOutwardFacingVectorsCircle(position, baseRadius* radiusMultiplier, baseAngleIncrement* angleMultiplier, y))
		{

			Firework* firework = new TimedFirework(this->GetPosition(), 1.0f);
			firework->SetVelocity(direction);
			firework->SetDamping(0.2f);
			vec.push_back(firework);
		}
	}

	return vec;
}

bool ConeFirework::IsValid()
{
	return this->GetVelocity().y > 0.0f;
}
