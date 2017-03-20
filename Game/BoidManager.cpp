
#include "Boid.h"
#include "GameData.h"
#include "DrawData.h"
#include "BoidManager.h"
#include "BoidsData.h"
#include <iostream>


BoidManager::BoidManager(int _numPrey, int _numMothers, int _numPredators, BoidsData * _preyData, BoidsData * _motherData, BoidsData * _predatorData, ID3D11Device * _pd3dDevice)
{
	m_pd3dDevice = _pd3dDevice;

	m_preyData = _preyData;
	m_predatorData = _predatorData;
	m_motherData = _motherData;


	 numPrey = _numPrey;
	 numPredators = _numPredators;
	 numMothers = _numMothers;

	m_boids.reserve(_numPrey + _numPredators + _numMothers);


	for (int i = 0; i < _numPrey; i++)
	{

		Boid* boid = new Boid(_preyData, _pd3dDevice);
		m_boids.push_back(boid);


	}

	for (int i = 0; i < _numPredators; i++)
	{
		Boid* boid = new Boid(_predatorData, _pd3dDevice);
		m_boids.push_back(boid);
	}

	for (int i = 0; i < _numMothers; i++)
	{
		Boid* boid = new Boid(_motherData, _pd3dDevice);
		m_boids.push_back(boid);
	}


	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i]->setBoids(m_boids);
	}
}

BoidManager::BoidManager(int _numPrey, int _numPredators, BoidsData* _preyData, BoidsData* _predatorData, ID3D11Device * _pd3dDevice)
{

	m_pd3dDevice = _pd3dDevice;

	m_preyData = _preyData;
	m_predatorData = _predatorData;

	numPrey = _numPrey;
	numPredators = _numPredators;

	m_boids.reserve(_numPrey + _numPredators);


	for (int i = 0; i < _numPrey; i++)
	{

		Boid* boid = new Boid(_preyData, _pd3dDevice);
		m_boids.push_back(boid);


	}

	for (int i = 0; i < _numPredators; i++)
	{
		Boid* boid = new Boid(_predatorData, _pd3dDevice);
		m_boids.push_back(boid);
	}


	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i]->setBoids(m_boids);
	}

}

BoidManager::BoidManager(int _numPrey, BoidsData* _preyData, ID3D11Device * _pd3dDevice)
{
	m_boids.reserve(_numPrey);

	m_pd3dDevice = _pd3dDevice;
	m_preyData = _preyData;

	numPrey = _numPrey;


	for (int i = 0; i < _numPrey; i++)
	{
		Boid* boid = new Boid(_preyData, _pd3dDevice);
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
		m_preyData = nullptr;
	}

	if (m_predatorData)
	{
		m_predatorData = nullptr;
	}

	if (m_motherData)
	{
		m_motherData = nullptr;
	}
}

void BoidManager::runBoids(GameData * _GD)
{
	for (std::vector<Boid*>::iterator it = m_boids.begin(); it != m_boids.end(); it++)
	{
		(*it)->Tick(_GD);
	}


	//if (*numBoids > m_boids.size())
	//{
	//	newBoid(m_pd3dDevice);
	//}


}

void BoidManager::drawBoids(DrawData * _DD)
{
	for (std::vector<Boid*>::iterator it = m_boids.begin(); it != m_boids.end(); it++)
	{
		(*it)->Draw(_DD);
	}
}



void BoidManager::newPrey(ID3D11Device * _pd3dDevice)
{
	Boid* new_boid = new Boid(m_preyData, _pd3dDevice);
	//numBoids++;
	m_boids.push_back(new_boid);
}

