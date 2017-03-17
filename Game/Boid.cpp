#include "Boid.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>


Boid::Boid(BoidsData* _data, ID3D11Device * _pd3dDevice)
{

	m_fudge = Matrix::CreateRotationY(XM_PIDIV2);


	GD = _pd3dDevice;
	m_data = _data;

	numVerts = 12;
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	indices = new WORD[numVerts];

	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}

	if (m_data->isPrey == true)
	{
		//top
		m_vertices[0].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[0].Pos = Vector3(0.0f, 0.0f, 0.0f);
		m_vertices[1].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[1].Pos = Vector3(0.0f, 0.0, 2.0f);
		m_vertices[2].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[2].Pos = Vector3(2.0f, -0.5, 1.0f);

		////back
		m_vertices[3].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[3].Pos = Vector3(0.0f, 0.0f, 0.0f);
		m_vertices[4].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[4].Pos = Vector3(0.0f, 0.0f, 2.0f);
		m_vertices[5].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[5].Pos = Vector3(0.0f, -1.0f, 1.0f);

		////right
		m_vertices[6].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[6].Pos = Vector3(0.0f, 0.0f, 2.0f);
		m_vertices[7].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[7].Pos = Vector3(0.0f, -1.0f, 1.0f);
		m_vertices[8].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[8].Pos = Vector3(2.0f, -0.5, 1.0f);

		////left
		m_vertices[9].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[9].Pos = Vector3(0.0f, 0.0, 0.0f);
		m_vertices[10].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[10].Pos = Vector3(0.0f, -1.0f, 1.0f);
		m_vertices[11].Color = Color(0.0f, 0.5f, 0.0f, 1.0f);
		m_vertices[11].Pos = Vector3(2.0f, -0.5f, 1.0f);


	}
	else if(m_data->isPrey == false)
	{

		//top
		m_vertices[0].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[0].Pos = Vector3(0.0f, 0.0f, 0.0f) * 5;
		m_vertices[1].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[1].Pos = Vector3(0.0f, 0.0, 2.0f) * 5;
		m_vertices[2].Color = Color(0.4f, 0.0f, 0.0f, 1.0f);
		m_vertices[2].Pos = Vector3(2.0f, -0.5, 1.0f) * 5;

		////back
		m_vertices[3].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[3].Pos = Vector3(0.0f, 0.0f, 0.0f) * 5;
		m_vertices[4].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[4].Pos = Vector3(0.0f, 0.0f, 2.0f) * 5;
		m_vertices[5].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[5].Pos = Vector3(0.0f, -1.0f, 1.0f) * 5;

		////right
		m_vertices[6].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[6].Pos = Vector3(0.0f, 0.0f, 2.0f) * 5;
		m_vertices[7].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[7].Pos = Vector3(0.0f, -1.0f, 1.0f) * 5;
		m_vertices[8].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[8].Pos = Vector3(2.0f, -0.5, 1.0f) * 5;

		////left
		m_vertices[9].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[9].Pos = Vector3(0.0f, 0.0, 0.0f) * 5;
		m_vertices[10].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[10].Pos = Vector3(0.0f, -1.0f, 1.0f) * 5;
		m_vertices[11].Color = Color(1.0f, 0.0f, 1.0f, 1.0f);
		m_vertices[11].Pos = Vector3(2.0f, -0.5f, 1.0f) * 5;
	}



	for (int i = 0; i < m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}

	BuildIB(GD, indices);
	BuildVB(GD, numVerts, m_vertices);


	delete[] indices;    //this is no longer needed as this is now in the index Buffer
	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	m_vertices = nullptr;



	m_size = 150;
	m_acc = Vector3::Zero;

	float angle = 0.1 + (rand() % (int)(359 - 0.1 + 1));
	m_vel = Vector3(cos(angle), cos(angle), sin(angle));

	m_up = Vector3::Transform(Vector3::Up, m_fudge.Invert() * m_worldMat) - m_pos;



	m_pos = Vector3((float)(rand() % m_size-5), (float)(rand() % m_size-5), (float)(rand() % m_size-5));
}

Boid::~Boid()
{
}

void Boid::Tick(GameData * _GD)
{

	flock();
	m_vel += m_acc *_GD->m_dt;
	m_vel = XMVector3ClampLength(m_vel, m_data->minSpeed, m_data->maxSpeed);
	m_pos += m_vel;


	Matrix scaleMat = Matrix::CreateScale(m_scale);
	Matrix rotTransMat = Matrix::CreateWorld(m_pos, m_vel , m_up);
	Matrix  transMat = Matrix::CreateTranslation(m_pos);
	m_worldMat = m_fudge * scaleMat * rotTransMat * transMat;

	m_acc = Vector3::Zero;
	Box();
}

void Boid::Draw(DrawData * _DD)
{
	VBGO::Draw(_DD);

	//std::cout << "boid drawn" << std::endl;
}

void Boid::setBoids(std::vector<Boid*> _boids)
{
	m_boids = _boids;
}

Vector3 Boid::Seperation()
{
	Vector3 steer = Vector3(Vector3::Zero);
	int count = 0;
	for (int i = 0; i < m_boids.size(); i++)
	{
		float distance = Vector3::Distance(m_pos, m_boids[i]->GetPos());
		if ((distance > 0) && (distance < m_data->seperation))
		{
			Vector3 difference = m_pos - m_boids[i]->GetPos();
			difference.Normalize();
			difference /= distance;
			steer += difference;
			count++;
		}

	}

	if (count > 0)
	{
		steer /= count;
	}


	if (steer != Vector3::Zero)
	{
		steer.Normalize();
		steer *= m_data->maxSpeed;
		steer -= m_vel;
		steer = XMVector3ClampLength(steer, m_data->minForce, m_data->maxForce);
		
	}

	return steer;
}

Vector3 Boid::Alignment()
{
	Vector3 sum = Vector3::Zero;

	int count = 0;

	for (int i = 0; i < m_boids.size(); i++)
	{
		float d = Vector3::Distance(m_pos, m_boids[i]->GetPos());
		if ((d > 0) && (d < m_data->neighbourDistance))
		{
			sum += m_boids[i]->GetVel();
			count++;
		}
	}


	if (count > 0)
	{
		sum /= (float)count;
		sum.Normalize();
		sum *= m_data->maxSpeed;

		Vector3 steer = sum - m_vel;
		steer = XMVector3ClampLength(steer, 0.0f, m_data->maxForce);
		return steer;
	}
	else
	{
		return Vector3::Zero;
	}
}

Vector3 Boid::Cohesion()
{
	Vector3 sum = Vector3::Zero;
	int count = 0;
	for (int i = 0; i < m_boids.size(); i++)
	{
		float d = Vector3::Distance(m_pos, m_boids[i]->GetPos());

		if ((d > 0) && (d < m_data->neighbourDistance))
		{
			sum += m_boids[i]->GetPos();
			count++;
		}
	}
	if (count > 0)
	{
		sum /= count;
		return Seek(sum);
	}
	else
	{
		return Vector3::Zero;
	}
}

Vector3 Boid::Repel()
{
	Vector3 rep = Vector3::Zero;

	if (m_data->isPrey)
	{

		for (int i = 0; i < m_boids.size(); i++)
		{
			float d = Vector3::Distance(m_pos, m_boids[i]->GetPos());
			
			if ((d > 0) && (d < m_data->neighbourDistance) && !m_boids[i]->getIsPrey())
			{
				rep = m_pos - m_boids[i]->GetPos();
				rep.Normalize();
				rep *= (m_data->repulsionForce / d);
			}
		}
	}

	return rep;
}

Vector3 Boid::Seek(Vector3 _target)
{
	Vector3 desired = _target - m_pos;
	desired.Normalize();

	desired *= m_data->maxSpeed;

	desired -= m_vel;
	desired = XMVector3ClampLength(desired, m_data->minSpeed, m_data->maxSpeed);

	Vector3 steer = desired - m_vel;
	steer = XMVector3ClampLength(steer, m_data->minForce, m_data->maxForce);

	return steer;
}

void Boid::Box()
{
	if (m_pos.x < 0)
	{
		m_pos.x += m_size;
	}
	else if (m_pos.x > m_size)
	{
		m_pos.x -= m_size;
	}

	if (m_pos.y < 0)
	{
		m_pos.y += m_size;
	}
	else if (m_pos.y > m_size)
	{
		m_pos.y -= m_size;
	}

	if (m_pos.z < 0)
	{
		m_pos.z += m_size;
	}
	else if (m_pos.z > m_size)
	{
		m_pos.z -= m_size;
	}
}

void Boid::applyForce(Vector3 _force)
{
	m_acc += _force;
}

void Boid::flock()
{
	Vector3 sep = Seperation();
	Vector3 ali = Alignment();
	Vector3 coh = Cohesion();
	Vector3 rep = Repel();

	ali *= 1.0;
	sep *= 1.5;
	coh *= 1.0;
	rep *= 1.0;

	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
	applyForce(rep);
}

void Boid::changeColour()
{

	float r = rand() % 1;
	float g = rand() % 1;
	float b = rand() % 1;



	for (int i = 0; i < numVerts; i++)
	{
		m_vertices[i].Color = Color(r, g, b, 0.0f);
	}

	//BuildVB(GD, numVerts, m_vertices);
}
