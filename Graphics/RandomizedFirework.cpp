#include "Fireworks.h"
#include "PhysicsUtils.h"

#include <glm/glm.hpp>

RandomizedFirework::RandomizedFirework(glm::vec3 position)
	: Firework(1.0f, position)
{

}

std::vector<Firework*> RandomizedFirework::BeginNextStage()
{
	std::vector<Firework*> vec;
	int numFireworks = getRandom(1.0f, 10.0f);
	for (int i = 0; i < numFireworks; i++)
	{
		glm::vec3 direction(getRandom(-1.0f, 1.0f), getRandom(-1.0f, 1.0f), getRandom(-1.0f, 1.0f));
		glm::normalize(direction);
		direction *= getRandom(8.0f, 20.0f);
		Firework* firework = new DistanceFirework(this->GetPosition());
		firework->SetVelocity(direction);
		vec.push_back(firework);
	}

	return vec;
}

bool RandomizedFirework::IsValid()
{
	return this->GetVelocity().y >= 0;
}
