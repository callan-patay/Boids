#pragma once

#include "gameobject.h"
#include "GameData.h"
#include "DrawData.h"


class CameraControl : public GameObject
{

public:
	CameraControl();
	~CameraControl();

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;




protected:






};