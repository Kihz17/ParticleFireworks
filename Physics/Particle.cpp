#include "Particle.h"

#include <glm/glm.hpp>

Particle::Particle(float mass, glm::vec3& position)
	: position(position), 
	velocity(0.f), 
	acceleration(0.f), 
	damping(0.995f), 
	appliedForce(0.f), 
	gravity(0.0f)
{
	this->hasGravity = false;

	this->radius = 3.5f;

	if (mass <= 0.f)
	{
		this->inverseMass = 0.0f;
	}
	else
	{
		this->inverseMass = 1.0f / mass;
	}
}

Particle::~Particle()
{

}

void Particle::SetDamping(float damping)
{
	this->damping = glm::clamp(damping, 0.f, 1.f);
}

float Particle::GetDamping() const
{
	return this->damping;
}

void Particle::SetMass(float mass)
{
	if (mass <= 0.0f)
	{
		this->inverseMass = 00.f;
	}
	else
	{
		this->inverseMass = 1.0f / mass;
	}
}

float Particle::GetMass() const
{
	if (this->inverseMass == 0.f)
	{
		return this->inverseMass;
	}
	return 1.0f / this->inverseMass;
}

float Particle::GetInverseMass() const
{
	return this->inverseMass;
}

glm::vec3 Particle::GetPosition() const
{
	return this->position;
}
void Particle::GetPosition(glm::vec3& position)
{
	position = this->position;
}
void Particle::SetPosition(const glm::vec3& position)
{
	this->position = position;
}

glm::vec3 Particle::GetVelocity() const
{
	return this->velocity;
}
void Particle::GetVelocity(glm::vec3& velocity)
{
	velocity = this->velocity;
}
void Particle::SetVelocity(const glm::vec3& velocity)
{
	this->velocity = velocity;
}

glm::vec3 Particle::GetAcceleration() const
{
	return this->acceleration;
}
void Particle::GetAcceleration(glm::vec3& acceleration)
{
	acceleration = this->acceleration;
}
void Particle::SetAcceleration(const glm::vec3& acceleration)
{
	this->acceleration = acceleration;
}

void Particle::SetUseCustomGravity(bool state)
{
	this->hasGravity = state;
}

void Particle::SetCustomGravity(glm::vec3 gravity)
{
	this->gravity = gravity;
}

glm::vec3 Particle::GetCustomGravity() const
{
	return this->gravity;
}

bool Particle::HasCustomGravity() const
{
	return this->hasGravity;
}

void Particle::ApplyForce(const glm::vec3& force)
{
	this->appliedForce += force;
}
void Particle::ClearAppliedForces()
{
	this->appliedForce.x = 0.0f;
	this->appliedForce.y = 0.0f;
	this->appliedForce.z = 0.0f;
}

void Particle::Integrate(float deltaTime)
{
	if (this->inverseMass == 0.0f) // Particle is static, do nothing
	{
		return;
	}

	this->position += this->velocity * deltaTime;

	// Scale velocity by acceleration applied force
	this->velocity += (this->acceleration + this->appliedForce * this->inverseMass) * deltaTime;

	// apply damping
	this->velocity *= glm::pow(this->damping, deltaTime);

	// clear applied forces
	ClearAppliedForces();
}

float Particle::GetSize() const
{
	return this->radius / 2.2f;
}

bool Particle::IsValid()
{
	return this->position.y > 0.0f;
}

float Particle::GetRadius() const
{
	return this->radius;
}

void Particle::SetRadius(float radius)
{
	this->radius = radius;
}

void Particle::OnCollision(Particle* collidedWith)
{
	// Does nothing here
}