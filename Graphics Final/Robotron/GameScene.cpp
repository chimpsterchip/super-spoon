//The GameScene class was not implemented
//
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2005 - 2016 Media Design School
//
//	File Name	:	GameScene.cpp
//	Description	:	Implementation of the GameScene.h
//	Author		:	Casey Chim
//	Mail		:	casey.chi6605@mediadesign.school.nz
//

#include "GameScene.h"


GameScene* GameScene::Instance = nullptr;

GameScene::GameScene()
{
	m_pGround = nullptr;
	m_iLevel = 0;
	m_iScene = 0;
	m_iMenuItem = 0;
	float CamYaw = 0.0f;
	float CamPitch = 0.0f;
	float CamSensitivity = 0.0f;
	m_fLastMouseX = 0.0f;
	m_fLastMouseY = 0.0f;
	srand(time(NULL));
}

GameScene::~GameScene()
{
	delete MainCamera;
	MainCamera = nullptr;

	delete MainLight;
	MainLight = nullptr;

	delete m_pGround;
	m_pGround = nullptr;
}

void GameScene::Init(HWND _name)
{
	name = _name;
	//Options
	m_bMSAA = true;
	m_bWireframe = true;
	m_bCull = true;
	m_bDepth = true;
	glDepthRangef(0.0f, 100.0f);
	//Shader Loading
	GLuint SkyboxProgram;
	GLuint ReflectionProgram;
	GLuint Model3DProgram;
	GLuint TerrainProgram;
	GLuint GeometryProgram;
	GLuint TessProgram;
	GLuint ToonProgram;
	ShaderLoader shaderLoader;
	SkyboxProgram = shaderLoader.CreateProgram("Skybox_Shader.vs", "Skybox_Shader.fs");
	ReflectionProgram = shaderLoader.CreateProgram("Reflection_Shader.vs", "Reflection_Shader.fs");
	Model3DProgram = shaderLoader.CreateProgram("3D_Model_Shader.vs", "3D_Model_Shader.fs");
	TerrainProgram = shaderLoader.CreateProgram("heightmap.vs", "heightmap.fs");
	GeometryProgram = shaderLoader.CreateProgram("Star.vs", "Star.gs", "Star.fs");
	TessProgram = shaderLoader.CreateProgram("Basic.vs", "Quad.tcs", "Quad.tes", "Basic.fs");
	ToonProgram = shaderLoader.CreateProgram("ToonShader.vs", "ToonShader.fs");

	WIDTH = 800;
	HEIGHT = 800;
	HalfWidth = WIDTH / 2;
	HalfHeight = HEIGHT / 2;

	LastTime = static_cast<GLfloat>(glutGet(GLUT_ELAPSED_TIME));
	//Setup Camera
	MainCamera = new CCamera(FLY, vec3(0.0f, 14.0f, 15.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGHT);
	CamSensitivity = 0.2f;

	//Setup Light
	MainLight = new CLight();
	MainLight->SetColor(vec3(0.5f, 0.5f, 0.5f));
	//Setup Light Model
	CModel* newModel = new CModel(vec3(0.0f, 7.0f, 0.0f), 36, MainCamera);
	newModel->init(CUBE, 0.5f, "Assets/textures/Green.png");
	MainLight->SetModel(newModel);
	
	//Setup Terrain
	hGenerator = new HeightGenerator();
	m_pTerrain = new CTerrain(L"Assets/heightmap/terrain.raw", "Assets/heightmap/rock.jpg", "Assets/heightmap/grass.jpg", "Assets/heightmap/sand.jpg", TerrainProgram, MainCamera, MainLight, hGenerator);

	//Setup Skybox
	m_pSkybox = new Cubemap(SkyboxProgram, MainCamera);

	m_pSphere = new ReflectionModel(ReflectionProgram, MainCamera, m_pSkybox);

	m_p3DCastle = new Model("Assets/models/castle2/castle.obj", MainCamera, Model3DProgram);
	m_pSuit = new Model("Assets/models/suit/nanosuit.obj", MainCamera, Model3DProgram);

	//Setup Geometry Shader Model
	m_pGeoModel = new GeometryModel(GeometryProgram, MainCamera);
	m_pGeoModel->setPosition(vec3(0.0f, 0.0f, 1.0f));

	//Setup Tesselation Model
	m_pTessModel = new TessModel(TessProgram, MainCamera);

	m_pBox = new CPlanet();

	//setup toon model
	toonModel = new ToonShadedModel(ToonProgram, MainCamera, MainLight, vec3(6.0f, 6.0f, 0.0f));
	toonModel->init(CUBE, 1.0f);
	

	//Create model
	newModel = new CModel(vec3(6.0f, 5.0f, 0.0f), 36, MainCamera);
	newModel->init(CUBE, 1.0f, "Assets/textures/Red.png");
	//Set the entities model
	m_pBox->SetModel(newModel);
	//Add entity to entity vector
	Entities.push_back(m_pBox);

	m_pMirror = new CPlanet();
	//Create model
	newModel = new CModel(vec3(6.0f, 4.0f, 0.0f), 36, MainCamera);
	newModel->init(QUAD, 4.0f, "Assets/textures/Green.png");
	//Set the entities model
	m_pMirror->SetModel(newModel);
	//Add entity to entity vector
	Entities.push_back(m_pMirror);

	m_pInvertedBox = new CPlanet();
	//Create model
	newModel = new CModel(vec3(6.0f, 3.0f, 0.0f), 36, MainCamera);
	newModel->init(CUBE, 1.0f, "Assets/textures/Red.png");
	//Set the entities model
	m_pInvertedBox->SetModel(newModel);
	//Add entity to entity vector
	Entities.push_back(m_pInvertedBox);
}

void GameScene::ClearScreen()
{
	for (auto iter = Labels.begin(); iter != Labels.end(); ++iter)
	{
		(*iter)->m_bActive = false;
	}
}

void GameScene::render()
{
	glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	MainLight->render();

	if (m_bCull)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	m_pSphere->render();

	if(m_bCull) glDisable(GL_CULL_FACE);

	m_pTessModel->render();

	toonModel->render();

	/*m_p3DCastle->Draw();
	m_p3DCastle->setPosition(vec3(0.0f, 5.95f, 0.0f));
	m_p3DCastle->setScale(vec3(0.5f, 0.5f, 0.5f));

	m_pSuit->Draw();
	m_pSuit->setPosition(vec3(0.0f, 6.0f, 12.0f));
	m_pSuit->setScale(vec3(0.1f, 0.1f, 0.1f));*/

	/*m_pBox->render();

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilMask(0xFF);
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);

	m_pMirror->render();
		
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDepthMask(GL_TRUE);

	m_pInvertedBox->render();

	glDisable(GL_STENCIL_TEST);*/

	m_pTerrain->render();

	m_pSkybox->render();

	//m_pGeoModel->render();

	glutSwapBuffers();
}

//Frame by frame update
void GameScene::update(unsigned char _keyState[255])
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error: " << err << endl;
	}

	//if (name == GetForegroundWindow() || GetWindow(name, GW_CHILD))
	//	glutWarpPointer(WIDTH / 2, HEIGHT / 2);
	GLfloat currentTime = static_cast<GLfloat>(glutGet(GLUT_ELAPSED_TIME));
	DeltaTime = (currentTime - LastTime) /1000.0f; //Time between last frame and current frame

	//Quick fix for zero deltatime on fast computers
	if (DeltaTime == 0.0f)
	{
		DeltaTime = 0.001;
	}
	LastTime = currentTime;
	
	//printf("\nDeltaTime: %f Time: %f LastTime: %f", DeltaTime, currentTime, LastTime);

	//Update camera
	if (MainCamera != nullptr)
	{
		MainCamera->update(_keyState, DeltaTime);
	}
	if (MainLight != nullptr)
	{
		MainLight->update(_keyState, DeltaTime);
	}
	if (_keyState[(unsigned char)'m'] == BUTTON_DOWN && Pressed)
	{
		if (m_bMSAA)
		{
			m_bMSAA = false;
			glDisable(GL_MULTISAMPLE);
		}
		else if (!m_bMSAA)
		{
			m_bMSAA = true;
			glEnable(GL_MULTISAMPLE);
		}
		Pressed = false;
	}
	if (_keyState[(unsigned char)'n'] == BUTTON_DOWN && Pressed)
	{
		if (m_bWireframe)
		{
			m_bWireframe = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (!m_bWireframe)
		{
			m_bWireframe = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		Pressed = false;
	}
	if (_keyState[(unsigned char)'b'] == BUTTON_DOWN && Pressed)
	{
		if (m_bCull)
		{
			m_bCull = false;
			glDisable(GL_CULL_FACE);
		}
		else if (!m_bCull)
		{
			m_bCull = true;
			glEnable(GL_CULL_FACE);
		}
		Pressed = false;
	}
	if (_keyState[(unsigned char)'v'] == BUTTON_DOWN && Pressed)
	{
		printf("Restarting Level");
		Init(name);
		Pressed = false;
	}
	if (_keyState[(unsigned char)'c'] == BUTTON_DOWN && Pressed)
	{
		if (m_bDepth)
		{
			m_bDepth = false;
			glDepthFunc(GL_ALWAYS);
		}
		else if (!m_bDepth)
		{
			m_bDepth = true;
			glDepthFunc(GL_LESS);
		}
		Pressed = false;
	}

	glutPostRedisplay();
}

void GameScene::mouseUpdate( int _iX, int _iY)
{
	if (name == GetForegroundWindow() || GetWindow(name, GW_CHILD))
	{	
		CamYaw += (_iX - HalfWidth) * CamSensitivity;
		CamPitch += (HalfHeight - _iY) * CamSensitivity;

		if (CamPitch > 89.0f)
			CamPitch = 89.0f;
		if (CamPitch < -89.0f)
			CamPitch = -89.0f;

		vec3 newCameraFront;
		newCameraFront.x = cos(radians(CamYaw)) * cos(radians(CamPitch));
		newCameraFront.y = sin(radians(CamPitch));
		newCameraFront.z = sin(radians(CamYaw)) * cos(radians(CamPitch));

		MainCamera->SetFront(normalize(newCameraFront));

		m_fLastMouseX = _iX;
		m_fLastMouseY = _iY;

		if (_iX != HalfWidth || _iY != HalfHeight) glutWarpPointer(HalfWidth, HalfHeight);
	}
}

void GameScene::SetPressed(bool _NewState){Pressed = _NewState;}

CCamera* GameScene::GetCamera(){return MainCamera;}

CLight* GameScene::GetLight(){return MainLight; }

GameScene& GameScene::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new GameScene();
	}

	return (*Instance);
}