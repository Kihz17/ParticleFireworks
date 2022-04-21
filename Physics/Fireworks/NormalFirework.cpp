#include "Fireworks.h"
#include "PhysicsUtils.h"

#include <glm/glm.hpp>

BaseFireworkNormal::BaseFireworkNormal(glm::vec3 position)
	: Firework(1.0f, position)
{
}

std::vector<Firework*> BaseFireworkNormal::BeginNextStage()
{
	std::vector<Firework*> vec;

	for (int i = 0; i < 30; i++) // Generate a bunch of new particles
	{
		glm::vec3 direction(getRandom(-1.0f, 1.0f), getRandom(-1.0f, 1.0f), getRandom(-1.0f, 1.0f));
		glm::normalize(direction);
		direction *= 25;
		Firework* firework = new TimedFirework(this->GetPosition(), 1.0f);
		firework->SetVelocity(direction);
		firework->SetDamping(1.0f);
		vec.push_back(firework);
	}

	return vec;
}

bool BaseFireworkNormal::IsValid()
{
	return this->GetVelocity().y > 0.0f;
}
