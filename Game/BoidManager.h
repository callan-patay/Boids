#pragma once
#include <vector>
#include <memory>
#include <d3d11_1.h> 
#include "Boid.h"
#include "gameobject.h"

//using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class GameData;
class DrawData;
class BoidsData;


class BoidManager : public GameObject
{
public:
	BoidManager(int _numPrey, int _numMothers, int _numPredators, BoidsData* _preyData, BoidsData* _motherData, BoidsData* _predatorData, ID3D11Device * _pd3dDevice);
	BoidManager(int _numPrey, int _numPredators, BoidsData* _preyData, BoidsData* _predatorData, ID3D11Device * _pd3dDevice);
	BoidManager(int _numPrey, BoidsData* _preyData, ID3D11Device * _pd3dDevice);
	virtual ~BoidManager();



	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;
	void newPrey(ID3D11Device * _pd3dDevice);

	int getNumPrey() { return numPrey; };
	int getNumPreds() { return numPredators; }
	int getNumMothers() { return numMothers; }

private:

	std::vector<Boid*> m_boids;
	int numPrey;
	int numPredators;
	int numMothers;

	BoidsData* m_preyData;
	BoidsData* m_predatorData;
	BoidsData* m_motherData;



	ID3D11Device* m_pd3dDevice;

};