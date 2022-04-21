#include "Fireworks.h"
#include "PhysicsUtils.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>

RotatingFirework::RotatingFirework(glm::vec3 position)
	: Firework(1.0f, position)
{
	this->currentAngle = 0.0f;
}

void RotatingFirework::Integrate(float deltaTime)
{
	glm::vec3 position = this->GetPosition();

	// change velocity so that the firework will rotate around the y axis at a fixed rate
	glm::vec3 newPos(position.x + (cos(this->currentAngle) * 25), position.y, position.z + (sin(this->currentAngle) * 25)); // Generate a new point away from the center using the current angle
	glm::vec3 direction = position - newPos; // Get the direction using the start and new point
	direction.y = this->GetVelocity().y;
	this->SetVelocity(direction);
	this->currentAngle += (6 * (M_PI / 180.0f));
	Particle::Integrate(deltaTime);
}

std::vector<Firework*> RotatingFirework::BeginNextStage()
{
	std::vector<Firework*> vec;
	glm::vec3 startPoint = this->GetPosition();

	float maxAngle = M_PI * 2.0f;
	float radiansIncrement = (25.0f * (M_PI / 180.0f));
	float radius = 25.0f;

	for (float angle = 0.0f; angle <= maxAngle; angle += radiansIncrement)
	{
		glm::vec3 newPos(startPoint.x + (cos(angle) * radius), 0, startPoint.z + (sin(angle) * radius)); // Generate a new point away from the center using the current angle
		glm::vec3 direction = startPoint - newPos; // Get the direction using the start and new point
		direction.y = 30.0f;
		glm::normalize(direction);
		direction *= 2.0f;

		Firework* firework = new RandomizedFirework(this->GetPosition());
		firework->SetVelocity(direction);
		firework->SetDamping(0.3f);
		vec.push_back(firework);
	}

	return vec;
}

bool RotatingFirework::IsValid()
{
	return this->GetVelocity().y > 0.0f;
}
