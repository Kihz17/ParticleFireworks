#include "Fireworks.h"
#include "PhysicsUtils.h"

Firework::Firework(float mass, glm::vec3 position)
	: Particle(mass, position)
{
	this->currentStage = 0;
}

Firework::~Firework()
{
}

