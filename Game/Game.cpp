#include "Game.h"
//DXTK headers
#include "SimpleMath.h"

//system headers
#include <windows.h>
#include <time.h>

//our headers
#include "ObjectList.h"
#include "GameData.h"
#include "drawdata.h"
#include "DrawData2D.h"
#include "BoidManager.h"
#include "Boid.h"
#include "BoidsData.h"
#include "CameraControl.h"
#include <AntTweakBar.h>



using namespace DirectX;
using namespace DirectX::SimpleMath;

Game::Game(ID3D11Device* _pd3dDevice, HWND _hWnd, HINSTANCE _hInstance) 
{
	//set up audio
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audioEngine.reset(new AudioEngine(eflags));

	//Create DirectXTK spritebatch stuff
	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"..\\Assets\\italic.spritefont"));

	//seed the random number generator
	srand((UINT)time(NULL));

	//Direct Input Stuff
	m_hWnd = _hWnd;
	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pMouse->SetCooperativeLevel(m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//create GameData struct and populate its pointers
	m_GD = new GameData;
	m_GD->m_keyboardState = m_keyboardState;
	m_GD->m_prevKeyboardState = m_prevKeyboardState;
	m_GD->m_GS = GS_PLAY_TPS_CAM;
	m_GD->m_mouseState = &m_mouseState;

	//set up DirectXTK Effects system
	m_fxFactory = new EffectFactory(_pd3dDevice);

	//Tell the fxFactory to look to the correct build directory to pull stuff in from
#ifdef DEBUG
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Debug");
#else
	((EffectFactory*)m_fxFactory)->SetDirectory(L"../Release");
#endif

	// Create other render resources here
	m_states = new CommonStates(_pd3dDevice);

	//init render system for VBGOs
	VBGO::Init(_pd3dDevice);

	//find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	//create a base camera


	//create a base light
	m_light = new Light(Vector3(.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.5f, 0.5f, 0.5f, 1.0f));
	m_GameObjects.push_back(m_light);



	TwInit(TW_DIRECT3D11, _pd3dDevice);
	TwWindowSize(width, height);

	CameraControl* m_cameraControl = new CameraControl();
	m_cameraControl->SetPos(Vector3(150.0f, 150.0f, 150.0));
	m_GameObjects.push_back(m_cameraControl);


	//add a secondary camera
	



	//creation of boids data objects for each type.
	BoidsData* m_preyData = new BoidsData();
	//prey have both repulsion and attraction
	m_preyData->repulsionForce = 50.0f;
	m_preyData->attractionForce = 0.60f;
	
	BoidsData* m_predatorData = new BoidsData();
	//predators have no repulsion and attraction, but higher speed and force and search radius
	m_predatorData->type = 2;
	m_predatorData->neighbourDistance = 50.0f;
	m_predatorData->maxSpeed = 2.0f;
	m_predatorData->maxForce = 1.0f;
	m_predatorData->seperation = 5.0f;


	BoidsData* m_motherData = new BoidsData();
	//mothers have repulsion from predators, and slightly higher speed than the prey to put them in front.
	m_motherData->type = 3;
	m_motherData->maxSpeed = 0.7;
	m_motherData->repulsionForce = 50.0f;

	

	m_cam = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3(75.0f, 75.0f, -75.0));
	m_cam->SetPos(Vector3(0.0f, 0.0f, -250.0f));
	m_GameObjects.push_back(m_cam);

	m_TPScam = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, m_cameraControl, Vector3::UnitY, Vector3(0.0f, 0.0f, 500.0f));
	m_GameObjects.push_back(m_TPScam);


	//different boid manager constructors, can be passed into the main game objects list to become part of the engine.
	//m_boidManager = std::make_unique<BoidManager>(1000, 3, m_preyData, m_predatorData, _pd3dDevice);
	BoidManager* m_boidManager = new BoidManager(Vector3::Zero, 1000, 10, 3, m_preyData, m_motherData, m_predatorData, _pd3dDevice);
	//m_boidManager = std::make_unique<BoidManager>(1000, m_preyData, _pd3dDevice);
	m_GameObjects.push_back(m_boidManager);

	TwBar *boidsBar = TwNewBar("Boids");
	TwDefine("Global help='Change the values to alter the properties of the boids.'");

	int barSize[2] = { 250, 550 };
	TwSetParam(boidsBar, NULL, "size", TW_PARAM_INT32, 2, barSize);


	if (m_boidManager->getNumPrey() > 0)
	{
		TwAddVarRW(boidsBar, "Prey Neighbour Dist", TW_TYPE_FLOAT, &m_preyData->neighbourDistance, "Group='Prey' min=1 max=100 step=0.5");	
		TwAddVarRW(boidsBar, "Prey Seperation", TW_TYPE_FLOAT, &m_preyData->seperation, "Group='Prey' min=1 max=100 step=0.5"); 
		TwAddVarRW(boidsBar, "Prey Max Speed", TW_TYPE_FLOAT, &m_preyData->maxSpeed, "Group='Prey' min=0.1 max=10 step=0.1");
		TwAddVarRW(boidsBar, "Prey Max Force", TW_TYPE_FLOAT, &m_preyData->maxForce, "Group='Prey' min=0.1 max=10 step=0.1");
		TwAddVarRW(boidsBar, "Prey Repulsion Force", TW_TYPE_FLOAT, &m_preyData->repulsionForce, "Group='Prey' min=0.1 max=100 step=0.1");
		TwAddVarRW(boidsBar, "Prey Attraction Force", TW_TYPE_FLOAT, &m_preyData->attractionForce, "Group='Prey' min=0.0 max= 1.0 step=0.01");

		TwAddVarRW(boidsBar, "Prey Alignment", TW_TYPE_FLOAT, &m_preyData->alignmentMultiplier, "Group='Prey Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Prey Separation", TW_TYPE_FLOAT, &m_preyData->separationMultiplier, "Group='Prey Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Prey Cohesion", TW_TYPE_FLOAT, &m_preyData->cohesionMultiplier, "Group='Prey Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Prey Repulsion", TW_TYPE_FLOAT, &m_preyData->repulsionMultiplier, "Group='Prey Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Prey Attraction", TW_TYPE_FLOAT, &m_preyData->attractionMultiplier, "Group='Prey Multipliers' min=0.01 max= 3.0 step=0.01");
	}
	if (m_boidManager->getNumMothers() > 0)
	{
		TwAddVarRW(boidsBar, "Mother Neighbour Dist", TW_TYPE_FLOAT, &m_motherData->neighbourDistance, "Group='Mother' min=1 max=100 step=0.5");
		TwAddVarRW(boidsBar, "Mother Seperation", TW_TYPE_FLOAT, &m_motherData->seperation, "Group='Mother' min=1 max=100 step=0.5");
		TwAddVarRW(boidsBar, "Mother Max Speed", TW_TYPE_FLOAT, &m_motherData->maxSpeed, "Group='Mother' min=0.1 max=10 step=0.1");
		TwAddVarRW(boidsBar, "Mother Max Force", TW_TYPE_FLOAT, &m_motherData->maxForce, "Group='Mother' min=0.1 max=10 step=0.1");
		TwAddVarRW(boidsBar, "Mother Repulsion Force", TW_TYPE_FLOAT, &m_motherData->repulsionForce, "Group='Mother' min=0.1 max=100 step=0.1");

		TwAddVarRW(boidsBar, "Mother Alignment", TW_TYPE_FLOAT, &m_motherData->alignmentMultiplier, "Group='Mother Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Mother Separation", TW_TYPE_FLOAT, &m_motherData->separationMultiplier, "Group='Mother Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Mother Cohesion", TW_TYPE_FLOAT, &m_motherData->cohesionMultiplier, "Group='Mother Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Mother Repulsion", TW_TYPE_FLOAT, &m_motherData->repulsionMultiplier, "Group='Mother Multipliers' min=0.01 max= 3.0 step=0.01");


	}
	if (m_boidManager->getNumPreds() > 0)
	{
		TwAddVarRW(boidsBar, "Pred Neighbour Dist", TW_TYPE_FLOAT, &m_predatorData->neighbourDistance, "Group='Predator' min=1 max=100 step=0.5");
		TwAddVarRW(boidsBar, "Pred Seperation", TW_TYPE_FLOAT, &m_predatorData->seperation, "Group='Predator' min=1 max=100 step=0.5");
		TwAddVarRW(boidsBar, "Pred Max Speed", TW_TYPE_FLOAT, &m_predatorData->maxSpeed, "Group='Predator' min=0.1 max=10 step=0.1");
		TwAddVarRW(boidsBar, "Pred Max Force", TW_TYPE_FLOAT, &m_predatorData->maxForce, "Group='Predator' min=0.1 max=10 step=0.1");

		TwAddVarRW(boidsBar, "Pred Alignment", TW_TYPE_FLOAT, &m_predatorData->alignmentMultiplier, "Group='Predator Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Pred Separation", TW_TYPE_FLOAT, &m_predatorData->separationMultiplier, "Group='Predator Multipliers' min=0.01 max= 3.0 step=0.01");
		TwAddVarRW(boidsBar, "Pred Cohesion", TW_TYPE_FLOAT, &m_predatorData->cohesionMultiplier, "Group='Predator Multipliers' min=0.01 max= 3.0 step=0.01");
	}

	//create DrawData struct and populate its pointers
	m_DD = new DrawData;
	m_DD->m_pd3dImmediateContext = nullptr;
	m_DD->m_states = m_states;
	m_DD->m_cam = m_cam;
	m_DD->m_light = m_light;

};



Game::~Game() 
{
	//delete Game Data & Draw Data
	delete m_GD;
	delete m_DD;

	//tidy up VBGO render system
	VBGO::CleanUp();

	//tidy away Direct Input Stuff
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pDirectInput)
	{
		m_pDirectInput->Release();
	}

	//get rid of the game objects here
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		delete (*it);
	}
	m_GameObjects.clear();


	//and the 2D ones
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		delete (*it);
	}
	m_GameObject2Ds.clear();

	//clear away CMO render system
	delete m_states;
	delete m_fxFactory;
	delete m_DD2D;
	TwTerminate();
};

bool Game::Tick() 
{
	//tick audio engine
	if (!m_audioEngine->Update())
	{
		// No audio device is active
		if (m_audioEngine->IsCriticalError())
		{
			//something has gone wrong with audio so QUIT!
			return false;
		}
	}

	//Poll Keyboard & Mouse
	ReadInput();

	//Upon pressing escape QUIT
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return false;
	}

	//lock the cursor to the centre of the window
	RECT window;
	GetWindowRect(m_hWnd, &window);
	//SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);

	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	m_GD->m_dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;

	//start to a VERY simple FSM
	switch (m_GD->m_GS)
	{
	case GS_ATTRACT:
		break;
	case GS_PAUSE:
		break;
	case GS_GAME_OVER:
		break;
	case GS_PLAY_MAIN_CAM:
	case GS_PLAY_TPS_CAM:
		PlayTick();
		break;
	}
	
	return true;
};

void Game::PlayTick()
{
	//upon space bar switch camera state
	//if ((m_keyboardState[DIK_SPACE] & 0x80) && !(m_prevKeyboardState[DIK_SPACE] & 0x80))
	//{
	//	if (m_GD->m_GS == GS_PLAY_MAIN_CAM)
	//	{
	//		m_GD->m_GS = GS_PLAY_TPS_CAM;
	//	}
	//	else
	//	{
	//		m_GD->m_GS = GS_PLAY_MAIN_CAM;
	//	}
	//}

	//m_boidManager->runBoids(m_GD);

	//update all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(m_GD);
	}
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->Tick(m_GD);
	}
}

void Game::Draw(ID3D11DeviceContext* _pd3dImmediateContext) 
{
	//set immediate context of the graphics device
	m_DD->m_pd3dImmediateContext = _pd3dImmediateContext;

	//set which camera to be used
	m_DD->m_cam = m_cam;
	if (m_GD->m_GS == GS_PLAY_TPS_CAM)
	{
		m_DD->m_cam = m_TPScam;
	}

	//m_boidManager->drawBoids(m_DD);

	TwDraw();

	//update the constant buffer for the rendering of VBGOs
	VBGO::UpdateConstantBuffer(m_DD);

	//draw all objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Draw(m_DD);
	}

	// Draw sprite batch stuff 
	m_DD2D->m_Sprites->Begin();
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->Draw(m_DD2D);
	}
	m_DD2D->m_Sprites->End();

	//drawing text screws up the Depth Stencil State, this puts it back again!
	_pd3dImmediateContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);
};



bool Game::ReadInput()
{
	//copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(unsigned char) * 256);

	//clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(unsigned char) * 256);
	ZeroMemory(&m_mouseState, sizeof(DIMOUSESTATE));

	// Read the keyboard device.
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	// Read the Mouse device.
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(hr))
	{
		// If the Mouse lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}