#include "Fireworks.h"
#include "PhysicsUtils.h"

TimedFirework::TimedFirework(glm::vec3 position, float time) :
	Firework(1.0f, position), lifeSpan(time), currentLiveFor(0.0f)
{
	this->SetUseCustomGravity(true);
	this->SetCustomGravity(glm::vec3(0.0f, -6.0f, 0.0f));
}

void TimedFirework::Integrate(float deltaTime)
{
	Particle::Integrate(deltaTime);
	currentLiveFor += deltaTime;
}

std::vector<Firework*> TimedFirework::BeginNextStage()
{
	std::vector<Firework*> vec; // Does nothing as we want this specific firework particle to do nothing after it expires
	return vec;
}

bool TimedFirework::IsValid()
{
	return this->currentLiveFor <= this->lifeSpan;
}