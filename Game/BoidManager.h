#pragma once
#include <vector>
#include <memory>
#include <d3d11_1.h> 
#include "Boid.h"

//using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class GameData;
class DrawData;
class BoidsData;


class BoidManager
{
public:
	BoidManager(int _numPrey, int _numPredators, BoidsData* _preyData, BoidsData* _predatorData, ID3D11Device * _pd3dDevice);
	BoidManager(int _numPrey, BoidsData* _preyData, ID3D11Device * _pd3dDevice);
	virtual ~BoidManager();



	void runBoids(GameData* _GD);
	void drawBoids(DrawData* _DD);
	void newPrey(ID3D11Device * _pd3dDevice);

	float getNeighbourDistance() { return	*m_neighbourDistance; };
	float getMaxSpeed() { return *m_maxSpeed; };
	float getMaxForce() { return *m_maxForce; };
	float getSeperation() { return *m_maxSpeed; };
	int getNumPrey() { return *numPrey; };

private:

	std::vector<Boid*> m_boids;
	int* numPrey;
	int* numPredators;
	float* m_neighbourDistance;
	float* m_maxSpeed;
	float* m_maxForce;
	float* m_seperation;
	BoidsData* m_preyData;
	BoidsData* m_predatorData;



	ID3D11Device* m_pd3dDevice;

};