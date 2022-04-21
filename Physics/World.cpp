#include "World.h"

World::World()
	: gravity(0.0f, -14.81f, 0.0f)
{

}

World::~World()
{

}

bool World::AddParticle(Particle* particle)
{
	if (!particle)
	{
		return false;
	}

	// Add particle to vector if we don't already have it
	std::vector<Particle*>::iterator itParticle = std::find(this->particles.begin(), this->particles.end(), particle);
	if (itParticle == this->particles.end())
	{
		this->particles.push_back(particle);
		return true;
	}
	return false;
}

bool World::RemoveParticle(Particle* particle)
{
	if (!particle)
	{
		return false; 
	}

	// Remove particle from vector if it is present
	std::vector<Particle*>::iterator itParticle = std::find(this->particles.begin(), this->particles.end(), particle);
	if (itParticle == this->particles.end())
	{
		return false;
	}

	this->particles.erase(itParticle);
	return true;
}

bool World::AddTarget(Particle* particle)
{
	if (!particle)
	{
		return false;
	}

	// Add particle to vector if we don't already have it
	std::vector<Particle*>::iterator itTarget = std::find(this->targets.begin(), this->targets.end(), particle);
	if (itTarget == this->targets.end())
	{
		this->targets.push_back(particle);
		return true;
	}
	return false;
}

bool World::RemoveTarget(Particle* particle)
{
	if (!particle)
	{
		return false;
	}

	// Remove particle from vector if it is present
	std::vector<Particle*>::iterator itParticle = std::find(this->targets.begin(), this->targets.end(), particle);
	if (itParticle == this->particles.end())
	{
		return false;
	}

	this->targets.erase(itParticle);
	return true;
}


void World::Tick(float deltaTime)
{
	IntegrateParticles(deltaTime);
}

void World::IntegrateParticles(float deltaTime)
{
	std::vector<Particle*>::iterator itParticle = this->particles.begin();
	while (itParticle != this->particles.end())
	{
		Particle* particle = (*itParticle);
		glm::vec3 gravity = particle->HasCustomGravity() ? particle->GetCustomGravity() : this->gravity;
		particle->ApplyForce(gravity * particle->GetMass());
		particle->Integrate(deltaTime);
		itParticle++;
	}
}
