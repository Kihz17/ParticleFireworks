#pragma once

#include <glm/vec3.hpp>

class Particle
{
public:
	Particle(float mass, glm::vec3& position);
	virtual ~Particle();

	Particle() = delete;
	Particle(Particle& other) = delete;
	Particle& operator=(Particle& other) = delete;

	virtual void SetDamping(float damping);
	virtual float GetDamping() const;

	virtual void SetMass(float mass);
	virtual float GetMass() const;
	virtual float GetInverseMass() const;

	virtual glm::vec3 GetPosition() const;
	virtual void GetPosition(glm::vec3& position);
	virtual void SetPosition(const glm::vec3& position);

	virtual glm::vec3 GetVelocity() const;
	virtual void GetVelocity(glm::vec3& velocity);
	virtual void SetVelocity(const glm::vec3& velocity);

	virtual glm::vec3 GetAcceleration() const;
	virtual void GetAcceleration(glm::vec3& acceleration);
	virtual void SetAcceleration(const glm::vec3& acceleration);

	virtual void SetUseCustomGravity(bool state);
	virtual void SetCustomGravity(glm::vec3 gravity);
	virtual glm::vec3 GetCustomGravity() const;
	virtual bool HasCustomGravity() const;

	virtual float GetRadius() const;
	virtual void SetRadius(float radius);

	virtual void ApplyForce(const glm::vec3& force);
	virtual void ClearAppliedForces();

	virtual void OnCollision(Particle* collidedWith);

	virtual void Integrate(float deltaTime);

	virtual float GetSize() const;
	virtual bool IsValid();

private:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 appliedForce;

	bool hasGravity;
	glm::vec3 gravity;

	float inverseMass;
	float damping;

	float radius;
};