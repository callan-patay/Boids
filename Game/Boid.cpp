#include "Boid.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>


Boid::Boid(Vector3 _pos, BoidsData* _data, ID3D11Device * _pd3dDevice)
{

	m_fudge = Matrix::CreateRotationY(XM_PIDIV2);


	m_data = _data;
	m_parentPos = _pos;
	numVerts = 12;
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	indices = new WORD[numVerts];

	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}

	//creates different boids of colour and size dependant on what type they are
	if (m_data->type == 1)
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
	else if (m_data->type == 2)
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
	else if (m_data->type == 3)
	{

		//top
		m_vertices[0].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[0].Pos = Vector3(0.0f, 0.0f, 0.0f) * 3;
		m_vertices[1].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[1].Pos = Vector3(0.0f, 0.0, 2.0f) * 3;
		m_vertices[2].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[2].Pos = Vector3(2.0f, -0.5, 1.0f) * 3;

		////back
		m_vertices[3].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[3].Pos = Vector3(0.0f, 0.0f, 0.0f) * 3;
		m_vertices[4].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[4].Pos = Vector3(0.0f, 0.0f, 2.0f) * 3;
		m_vertices[5].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[5].Pos = Vector3(0.0f, -1.0f, 1.0f) * 3;

		////right
		m_vertices[6].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[6].Pos = Vector3(0.0f, 0.0f, 2.0f) * 3;
		m_vertices[7].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[7].Pos = Vector3(0.0f, -1.0f, 1.0f) * 3;
		m_vertices[8].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[8].Pos = Vector3(2.0f, -0.5, 1.0f) * 3;

		////left
		m_vertices[9].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[9].Pos = Vector3(0.0f, 0.0, 0.0f) * 3;
		m_vertices[10].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[10].Pos = Vector3(0.0f, -1.0f, 1.0f) * 3;
		m_vertices[11].Color = Color(0.2f, 0.5f, 1.0f, 1.0f);
		m_vertices[11].Pos = Vector3(2.0f, -0.5f, 1.0f) * 3;
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

	BuildIB(_pd3dDevice, indices);
	BuildVB(_pd3dDevice, numVerts, m_vertices);


	delete[] indices;    //this is no longer needed as this is now in the index Buffer
	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	m_vertices = nullptr;


	//allocates bounding box size







	//creates a position within a 150 sized box
	m_size =  150;
	int sizeX = _pos.x + m_size;
	int sizeY = _pos.y + m_size;
	int sizeZ = _pos.z + m_size;
	

	//initialises the boid in a random location with a random velocity to get them started.
	float angle = 0.1 + (rand() % (int)(359 - 0.1 + 1));
	m_pos = Vector3((float)(rand() % sizeX - _pos.x) + _pos.x, (float)(rand() % sizeX - _pos.y) + _pos.y, (float)(rand() % sizeX - _pos.z) + _pos.z);
	m_vel = Vector3(cos(angle), cos(angle), sin(angle));
	m_acc = Vector3::Zero;
	m_up = Vector3::Transform(Vector3::Up, m_fudge.Invert() * m_worldMat) - m_pos;
}
Boid::~Boid()
{
	for (int i = 0; i < m_boids.size(); i++)
	{
		m_boids[i] = nullptr;
	}


	m_data = nullptr;
}

void Boid::Tick(GameData * _GD)
{
	//apply acceleration
	flock();

	//applys boid new vector to position and updates velocity
	m_vel += m_acc *_GD->m_dt;
	m_vel = XMVector3ClampLength(m_vel, m_data->minSpeed, m_data->maxSpeed);
	m_pos += m_vel;

	//pointers boid in direction of movement
	Matrix scaleMat = Matrix::CreateScale(m_scale);
	Matrix rotTransMat = Matrix::CreateWorld(m_pos, m_vel , m_up);
	Matrix  transMat = Matrix::CreateTranslation(m_pos);
	m_worldMat = m_fudge * scaleMat * rotTransMat * transMat;

	//reset acceleration each tick
	m_acc = Vector3::Zero;

	//keeps boids in the box wherever the boids manager is placed.
	Box();
}

void Boid::Draw(DrawData * _DD)
{
	VBGO::Draw(_DD);
}

void Boid::setBoids(std::vector<Boid*> _boids)
{
	//gives boid access to all the boids in boid manager
	m_boids = _boids;
}

Vector3 Boid::Seperation()
{
	Vector3 steer = Vector3(Vector3::Zero);
	int count = 0;
	for (int i = 0; i < m_boids.size(); i++)
	{
		float distance = Vector3::Distance(m_pos, m_boids[i]->GetPos());
		//checks if boid is within their separation distance
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
		//if there is a boid in separation distance, get the average of all boids within separation distance
		steer /= count;
	}


	if (steer != Vector3::Zero)
	{
		//limit the force applied
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
			//if boid is within neighbour distance, get average velocity
			sum += m_boids[i]->GetVel();
			count++;
		}
	}

	if (count > 0)
	{
		//limits force
		sum /= (float)count;
		sum.Normalize();
		sum *= m_data->maxSpeed;

		Vector3 steer = sum - m_vel;
		steer = XMVector3ClampLength(steer, m_data->minForce, m_data->maxForce);
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
		//if boid is within another boid in its neighbour distance, get average position
		if ((d > 0) && (d < m_data->neighbourDistance))
		{
			sum += m_boids[i]->GetPos();
			count++;
		}
	}
	if (count > 0)
	{
		//if boid is within neighbour distance, average it relative to current boids position.
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

	for (int i = 0; i < m_boids.size(); i++)
	{
		float d = Vector3::Distance(m_pos, m_boids[i]->GetPos());
		//if the boid within neighbour distance is a predator, apply force away from it.
		if ((d > 0) && (d < m_data->neighbourDistance) && m_boids[i]->getType() == 2)
		{
			rep = m_pos - m_boids[i]->GetPos();
			rep.Normalize();
			rep *= (m_data->repulsionForce / d);
		}
	}

	return rep;
}

Vector3 Boid::Attract()
{
	Vector3 att = Vector3::Zero;

	for (int i = 0; i < m_boids.size(); i++)
	{
		float d = Vector3::Distance(m_pos, m_boids[i]->GetPos());
		//if boid found is a mother, move towards mothers position
		if ((d > 0) && (d < m_data->neighbourDistance) && m_boids[i]->getType() == 3)
		{
			att = m_pos - m_boids[i]->GetPos();
			att.Normalize();
			att *= -m_data->attractionForce;
		}
	}

	return att;
}

Vector3 Boid::Seek(Vector3 _target)
{
	//used for cohesion, averages position against the boids own position.
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
	//keeps the boids within a box, if they hit a wall they spawn the other side of the box.
	if (m_pos.x < m_parentPos.x)
	{
		m_pos.x += m_size;
	}
	else if (m_pos.x > m_parentPos.x + 150)
	{
		m_pos.x -= m_size;
	}

	if (m_pos.y < m_parentPos.y)
	{
		m_pos.y += m_size;
	}
	else if (m_pos.y > m_parentPos.y + 150)
	{
		m_pos.y -= m_size;
	}

	if (m_pos.z < m_parentPos.z)
	{
		m_pos.z += m_size;
	}
	else if (m_pos.z > m_parentPos.y + 150)
	{
		m_pos.z -= m_size;
	}
}

void Boid::applyForce(Vector3 _force)
{
	//helper function
	m_acc += _force;
}

void Boid::flock()
{
	//boid forces calculated
	Vector3 sep = Seperation();
	Vector3 ali = Alignment();
	Vector3 coh = Cohesion();
	Vector3 rep = Vector3::Zero;
	Vector3 att = Vector3::Zero;

	//prey and mothers can repel away from predators
	if (m_data->type != 2)
	{
		rep = Repel();
	}

	//prey are only attracted to mothers
	if (m_data->type == 1)
	{
		att = Attract();
	}

	//allows scalability of boid force
	ali *= m_data->alignmentMultiplier;
	sep *= m_data->separationMultiplier;
	coh *= m_data->cohesionMultiplier;
	rep *= m_data->repulsionMultiplier;
	att *= m_data->attractionMultiplier;

	//applies all boid logic to the acceleration.
	applyForce(sep);
	applyForce(ali);
	applyForce(coh);
	applyForce(rep);
	applyForce(att);
}
