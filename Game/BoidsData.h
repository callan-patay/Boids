#pragma once


struct BoidsData
{
	float maxForce = 0.3f;
	float minForce = 0.0f;
	float maxSpeed = 0.5f;
	float minSpeed = 0.0f;
	float neighbourDistance = 20.0f;
	float seperation = 5.0f;
	float repulsionForce = 0.0f;
	float attractionForce = 0.0f;
	float alignmentMultiplier = 1.0f;
	float separationMultiplier = 1.5f;
	float cohesionMultiplier = 1.0f;
	float repulsionMultiplier = 1.0f;
	float attractionMultiplier = 1.0f;
	int type = 1;
};

