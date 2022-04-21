#pragma once

#include "Particle.h";

#include <vector>

class Firework : public Particle
{
public:
	Firework(float mass, glm::vec3 position);
	virtual ~Firework();

	virtual bool IsValid() = 0;

	// Begin this firework's next stage
	virtual std::vector<Firework*> BeginNextStage() = 0;

protected:
	int currentStage;
};

// Represents a firework that simply goes upwards and dissapears when its y velocity reaches 0
class BaseFireworkNormal : public Firework
{
public:
	BaseFireworkNormal(glm::vec3 position);

	virtual std::vector<Firework*> BeginNextStage();

	virtual bool IsValid();
};

// Represents a firework with a cone explosion
class ConeFirework : public Firework
{
public:
	ConeFirework(glm::vec3 position);

	virtual std::vector<Firework*> BeginNextStage();

	virtual bool IsValid();
};

// Represents a firework particle that will be removed once it is a certain distance away from its original position
class DistanceFirework : public Firework
{
public:
	DistanceFirework(glm::vec3 position);

	virtual std::vector<Firework*> BeginNextStage();

	virtual bool IsValid();

private:
	glm::vec3 originalPosition;
};

// Represents a firework particle that will be removed once it has lived for a certain time
class TimedFirework : public Firework
{
public:
	TimedFirework(glm::vec3 position, float time);

	virtual void Integrate(float deltaTime);

	virtual std::vector<Firework*> BeginNextStage();

	virtual bool IsValid();

private:
	float lifeSpan, currentLiveFor;
};

// Represents a firework that will rotate around the y axis
class RotatingFirework : public Firework
{
public:
	RotatingFirework(glm::vec3 position);

	virtual void Integrate(float deltaTime);

	virtual std::vector<Firework*> BeginNextStage();

	virtual bool IsValid();

private:
	float currentAngle;
};

// Represents a fire work that will spawn other fireworks randomly
class RandomizedFirework : public Firework
{
public:
	RandomizedFirework(glm::vec3 position);

	virtual std::vector<Firework*> BeginNextStage();

	virtual bool IsValid();
};

