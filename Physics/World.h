#pragma once

#include <vector>

#include "Particle.h"

class World
{
public:
	World();
	virtual ~World();

	bool AddParticle(Particle* particle);
	bool RemoveParticle(Particle* particle);

	bool AddTarget(Particle* particle);
	bool RemoveTarget(Particle* particle);

	void Tick(float deltaTime);

	void IntegrateParticles(float deltaTime);

private:
	std::vector<Particle*> particles;
	std::vector<Particle*> targets;
	glm::vec3 gravity;
};