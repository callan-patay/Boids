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
	bool isPrey = true;
};

