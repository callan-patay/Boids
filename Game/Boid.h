#pragma once

#include "DrawData.h"
#include "GameData.h"
#include "vertex.h"
#include "BoidsData.h"
#include "VBGO.h"
#include <vector>
#include <math.h>

class Boid : public VBGO
{
public:
	Boid(Vector3 _pos, BoidsData* _data, ID3D11Device* _pd3dDevice);
	virtual ~Boid();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD) override;
	void setBoids(std::vector<Boid*> _boids);
	Vector3 Seperation();
	Vector3 Alignment();
	Vector3 Cohesion();
	Vector3 Repel();
	Vector3 Attract();
	Vector3 Seek(Vector3 _target);
	void Box();
	void applyForce(Vector3 _force);
	void flock();



	int getType() { return m_data->type; }


private:
	std::vector<Boid*> m_boids;
	int numVerts;
	myVertex* m_vertices;
	WORD* indices;
	int m_size;
	Vector3 m_parentPos;
	Vector3 m_up;

	BoidsData* m_data;



};