
#include "Boid.h"
#include "GameData.h"
#include "DrawData.h"
#include "BoidManager.h"
#include "BoidsData.h"
#include <iostream>


BoidManager::BoidManager(Vector3 _pos, int _numPrey, int _numMothers, int _numPredators, BoidsData * _preyData, BoidsData * _motherData, BoidsData * _predatorData, ID3D11Device * _pd3dDevice)
{
	m_pos = _pos;

	//if null ptr's for the data objects are passed, create standard data objects
	if (!_preyData)
	{
		m_preyData = new BoidsData();
		m_preyData->repulsionForce = 50.0f;
		m_preyData->attractionForce = 0.60f;
	}
	else
	{
		m_preyData = _preyData;
	}

	if (!_predatorData)
	{
		m_predatorData = new BoidsData();
		m_predatorData->type = 2;
		m_predatorData->neighbourDistance = 50.0f;
		m_predatorData->maxSpeed = 2.0f;
		m_predatorData->maxForce = 1.0f;
		m_predatorData->seperation = 5.0f;
	}
	else
	{
		m_predatorData = _predatorData;
	}

	if (!_motherData)
	{
		m_motherData = new BoidsData();
		m_motherData->type = 3;
		m_motherData->maxSpeed = 0.7;
		m_motherData->repulsionForce = 50.0f;
	}
	else
	{
		m_motherData = _motherData;
	}



	 numPrey = _numPrey;
	 numPredators = _numPredators;
	 numMothers = _numMothers;

	m_boids.reserve(_numPrey + _numPredators + _numMothers);

	//creates the  different kinds of boids based on parameters passed

	for (int i = 0; i < _numPrey; i++)
	{

		Boid* boid = new Boid(m_pos, m_preyData, _pd3dDevice);
		m_boids.push_back(boid);

	}

	for (int i = 0; i < _numPredators; i++)
	{
		Boid* boid = new Boid(m_pos, m_predatorData, _pd3dDevice);
		m_boids.push_back(boid);
	}

	for (int i = 0; i < _numMothers; i++)
	{
		Boid* boid = new Boid(m_pos, m_motherData, _pd3dDevice);
		m_boids.push_back(boid);
	}

	//gives boids access to every other boid

	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i]->setBoids(m_boids);
	}
}

BoidManager::BoidManager(Vector3 _pos, int _numPrey, int _numPredators, BoidsData* _preyData, BoidsData* _predatorData, ID3D11Device * _pd3dDevice)
{
	//does the same as the first constructor
	m_pos = _pos;
	if (!_preyData)
	{
		m_preyData = new BoidsData();
		m_preyData->repulsionForce = 50.0f;
		m_preyData->attractionForce = 0.60f;
	}
	else
	{
		m_preyData = _preyData;
	}

	if (!_predatorData)
	{
		m_predatorData = new BoidsData();
		m_predatorData->type = 2;
		m_predatorData->neighbourDistance = 50.0f;
		m_predatorData->maxSpeed = 2.0f;
		m_predatorData->maxForce = 1.0f;
		m_predatorData->seperation = 5.0f;
	}
	else
	{
		m_predatorData = _predatorData;
	}

	numPrey = _numPrey;
	numPredators = _numPredators;

	m_boids.reserve(_numPrey + _numPredators);


	for (int i = 0; i < _numPrey; i++)
	{

		Boid* boid = new Boid(m_pos, _preyData, _pd3dDevice);
		m_boids.push_back(boid);


	}

	for (int i = 0; i < _numPredators; i++)
	{
		Boid* boid = new Boid(m_pos, _predatorData, _pd3dDevice);
		m_boids.push_back(boid);
	}


	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i]->setBoids(m_boids);
	}

}

BoidManager::BoidManager(Vector3 _pos, int _numPrey, BoidsData* _preyData, ID3D11Device * _pd3dDevice)
{
	m_boids.reserve(_numPrey);
	m_pos = _pos;
	//does the same as the previous constructor
	if (!_preyData)
	{
		m_preyData = new BoidsData();
		m_preyData->repulsionForce = 50.0f;
		m_preyData->attractionForce = 0.60f;
	}
	else
	{
		m_preyData = _preyData;
	}

	numPrey = _numPrey;


	for (int i = 0; i < _numPrey; i++)
	{
		Boid* boid = new Boid(m_pos, _preyData, _pd3dDevice);
		m_boids.push_back(boid);

		std::cout << "made boid: " << i << std::endl;
	}

	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i]->setBoids(m_boids);
	}


}

BoidManager::~BoidManager()
{
	if (m_preyData)
	{
		delete m_preyData;
		m_preyData = nullptr;
	}

	if (m_predatorData)
	{
		delete m_predatorData;
		m_predatorData = nullptr;
	}

	if (m_motherData)
	{
		delete m_motherData;
		m_motherData = nullptr;
	}

	for (int i = 0; i < m_boids.size(); i++)
	{
		delete m_boids[i];

		m_boids[i] = nullptr;
	}


}

void BoidManager::Tick(GameData * _GD)
{
	//runs the boids logic
	for (std::vector<Boid*>::iterator it = m_boids.begin(); it != m_boids.end(); it++)
	{
		(*it)->Tick(_GD);
	}
}

void BoidManager::Draw(DrawData * _DD)
{
	//draws the boids
	for (std::vector<Boid*>::iterator it = m_boids.begin(); it != m_boids.end(); it++)
	{
		(*it)->Draw(_DD);
	}
}
