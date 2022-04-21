#include "Fireworks.h"
#include "PhysicsUtils.h"

DistanceFirework::DistanceFirework(glm::vec3 position)
	: Firework(1.0f, position), originalPosition(position)
{
	this->SetUseCustomGravity(true);
	this->SetCustomGravity(glm::vec3(0.0f, -6.0f, 0.0f));
}

std::vector<Firework*> DistanceFirework::BeginNextStage()
{
	std::vector<Firework*> vec;
	return vec;
}

bool DistanceFirework::IsValid()
{
	glm::vec3 diff = this->GetPosition() - this->originalPosition;
	return sqrt((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z)) <= 20.0f; // Removes this particle once it is a certain distance away from its spawn point
}
