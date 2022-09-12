/* 
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)
*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"
#include <math.h>
#include <chrono>
#include <ctime>

// Game includes
#include "Torus.h"
#include "Cube.h"
#include "FrameBufferObject.h"
#include "CatmullRom.h"
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "SoundSource.h"

// Constructor
Game::Game()
{
	m_pCubesPositions = NULL;
	m_pRocksPositions = NULL;
	m_pTorus = NULL;
	m_pTorus2 = NULL;
	m_pCubes = NULL;
	m_pFBOTopView = NULL;
	m_pFBO = NULL;
	m_pPath = NULL;
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pCameraTopView = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pPlanarScene = NULL;
	m_pFtFont = NULL;
	m_pHUD = NULL;
	m_pBarrelMesh = NULL;
	m_pHorseMesh = NULL;
	m_pSphere = NULL;
	m_pModule = NULL;
	m_pSubmarine = NULL;
	m_pRockA = NULL;
	m_pRockB = NULL;
	m_pRockC = NULL;
	m_pRockD = NULL;
	m_pRockE = NULL;
	m_pBarrels = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pSoundSource = NULL;
	m_pSubmarineSoundSource = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.01f;
	m_cameraRotation = 0.0f;
	m_viewMode = third;
	m_filterControl = 0.0f;
	m_cubeRot = 0.0f;
	m_collectedCubes = 0;
	m_submarinePosition = glm::vec3(71.0f, 5.0f, 71.0f);
	m_submarineSide = glm::vec3(0.0f, 0.0f, 0.0f);
	m_submarineOrientation = glm::mat4(1.0f);
	m_submarineSpeed = 0.0f;
	m_torusDistance = 1250.0f;
	m_torusPosition = glm::vec3(1025.936890, 7.039076, 1003.559265);
	m_torusOrientation = glm::mat4(1.0f);
	m_torusDistance2 = 6000.0f;
	m_torusPosition2 = glm::vec3(0.0, 0.0, 0.0);
	m_torusOrientation2 = glm::mat4(1.0f);
	m_torusSpeed = false;
	m_torusSpeedTime = 0.0;
	m_lightOn = true;
	m_startTime = std::chrono::system_clock::now();
	m_start = false;
	m_timePlayed = 0.0;
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCubesPositions;
	delete m_pRocksPositions;
	delete m_pTorus;
	delete m_pTorus2;
	delete m_pFBO;
	delete m_pFBOTopView;
	delete m_pPath;
	delete m_pCamera;
	delete m_pCameraTopView;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pPlanarScene;
	delete m_pFtFont;
	delete m_pHUD;
	delete m_pBarrelMesh;
	delete m_pHorseMesh;
	delete m_pRockA;
	delete m_pRockB;
	delete m_pRockC;
	delete m_pRockD;
	delete m_pRockE;
	delete m_pBarrels;
	delete m_pSphere;
	delete m_pModule;
	delete m_pSubmarine;
	delete m_pAudio;
	delete m_pSoundSource;
	delete m_pSubmarineSoundSource;

	if (m_pCubes != NULL) {
		for (unsigned int i = 0; i < m_pCubes->size(); i++)
			delete (*m_pCubes)[i];
	}
	delete m_pCubes;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	// Create objects
	m_pCubesPositions = new vector<glm::vec3>{ glm::vec3(1500, 5 + 15, 400),  glm::vec3(500, 5 + 15, 1500), glm::vec3(1250, 5 + 15, 2800), glm::vec3(-500, -80 + 15, 2300), glm::vec3(-1000, 38 + 15, 1000) };
	m_pRocksPositions = new vector<glm::vec3>{ glm::vec3(873.624512f, 4.810508f, -26.400465f), glm::vec3(1531.706421f, 17.560579f, 606.644714f),
				glm::vec3(438.523590f, -14.528829f, 1204.209595f), glm::vec3(555.962891f, 6.798330f, 1565.175903f),
				glm::vec3(1728.013306f, 5.000000f, 2226.925049f), glm::vec3(700.572266f, 5.737399f, 2330.748291f),
				glm::vec3(73.245956f, -1.213879f, 1738.892456f), glm::vec3(-184.137146f, -2.358431f, 1756.539307f),
				glm::vec3(-431.467255f, 30.414373f, 1356.399170f), glm::vec3(-665.115173f, 25.934746f, 560.476746f), 
				glm::vec3(623.466553f, 4.060357f, -75.448341f), glm::vec3(1500.844116f, 49.944080f, 1097.953247),
				glm::vec3(650.873474, -18.815233, 958.710205), glm::vec3(1054.558472f, 4.995919f, 1600.697876),
				glm::vec3(1639.191772f, 5.000000f, 1856.910767f), glm::vec3(1170.262329f, 5.065478f, 2768.829834),
				glm::vec3(299.571106f, 3.327008f, 1892.371826f), glm::vec3(-590.278748f, -83.529861f, 2137.438232f),
				glm::vec3(-879.161499f, 36.702816f, 1120.569824f), glm::vec3(-115.698029f, 7.162012f, 49.071007f),
				glm::vec3(1115.389038, 0.502003, 65.936493), glm::vec3(1483.617432, 0.790963, 366.540161),
				glm::vec3(1263.640625, 25.391977, 1067.369141), glm::vec3(803.609314f, 1.223301f, 1588.468018f),
				glm::vec3(1640.337402, 0.000000, 2466.988037), glm::vec3(910.072693, 0.607118, 2518.150391),
				glm::vec3(-287.545197, -40.165909, 1683.459717), glm::vec3(-351.697754, -70.539474, 2224.338379),
				glm::vec3(-220.977585, 18.909378, 1514.444824), glm::vec3(-857.451843, 20.356800, 744.549500),
				glm::vec3(497.746826, 0.730947, -56.333618), glm::vec3(1580.294800, 30.003113, 858.630127),
				glm::vec3(478.687744, -20.206961, 1077.801880), glm::vec3(929.448914, 0.237438, 1574.761475),
				glm::vec3(1508.359497, 0.000000, 1631.927124), glm::vec3(1405.232422, 0.000000, 2740.204834),
				glm::vec3(363.576294, 0.090534, 1977.103271), glm::vec3(-544.021667, -80.858459, 2278.773926),
				glm::vec3(-655.671631, 25.626198, 1261.074219), glm::vec3(-509.438385, 26.582850, 381.154083) };
	m_pTorus = new CTorus;
	m_pTorus2 = new CTorus;
	m_pCubes = new vector <CCube *>;
	m_pFBO = new CFrameBufferObject;
	m_pFBOTopView = new CFrameBufferObject;
	m_pPath = new CCatmullRom;
	m_pCamera = new CCamera;
	m_pCameraTopView = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
	m_pPlanarTerrain = new CPlane;
	m_pPlanarScene = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	m_pHUD = new CFreeTypeFont;
	m_pBarrelMesh = new COpenAssetImportMesh;
	m_pHorseMesh = new COpenAssetImportMesh;
	m_pRockA = new COpenAssetImportMesh;
	m_pRockB = new COpenAssetImportMesh;
	m_pRockC = new COpenAssetImportMesh;
	m_pRockD = new COpenAssetImportMesh;
	m_pRockE = new COpenAssetImportMesh;
	m_pBarrels = new COpenAssetImportMesh;
	m_pSphere = new CSphere;
	m_pModule = new COpenAssetImportMesh;
	m_pSubmarine = new COpenAssetImportMesh;
	m_pAudio = new CAudio;
	m_pSoundSource = new CSoundSource;
	m_pSubmarineSoundSource = new CSoundSource;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	//Create a FBO with the screen width and height
	m_pFBO->Create(width, height);

	//Create a FBO for the top view with the screen width and height
	m_pFBOTopView->Create(width, height);

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height); 
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) width / (float) height, 0.5f, 5000.0f);

	// Set the orthographic and perspective projection matrices based on the image size for the top view camera
	m_pCameraTopView->SetOrthographicProjectionMatrix(width, height);
	m_pCameraTopView->SetPerspectiveProjectionMatrix(45.0f, (float)width / (float)height, 0.5f, 5000.0f);
	//Set the position, view and up vectors for the top view camera 
	m_pCameraTopView->Set(glm::vec3(300.0, 3000.0, 1200.0), glm::vec3(300.0, 0.0, 1200.0), glm::vec3(1.0, 0.0, 0.0));

	//Create Cubes
	for (int i = 0; i < m_pCubesPositions->size(); i++) {
		CCube *cube = new CCube;
		cube->Create("resources\\textures\\Wood_Pattern_001_basecolor.jpg");
		m_pCubes->push_back(cube);
	}

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("shader.vert");
	sShaderFileNames.push_back("shader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("blurShader.vert");
	sShaderFileNames.push_back("blurShader.frag");
	sShaderFileNames.push_back("topviewShader.vert");
	sShaderFileNames.push_back("topviewShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\"+sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create the blur shader program
	CShaderProgram *pBlurProgram = new CShaderProgram;
	pBlurProgram->CreateProgram();
	pBlurProgram->AddShaderToProgram(&shShaders[4]);
	pBlurProgram->AddShaderToProgram(&shShaders[5]);
	pBlurProgram->LinkProgram();
	m_pShaderPrograms->push_back(pBlurProgram);

	// Create the blur shader program
	CShaderProgram *pTopViewProgram = new CShaderProgram;
	pTopViewProgram->CreateProgram();
	pTopViewProgram->AddShaderToProgram(&shShaders[6]);
	pTopViewProgram->AddShaderToProgram(&shShaders[7]);
	pTopViewProgram->LinkProgram();
	m_pShaderPrograms->push_back(pTopViewProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);
	
	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	//Create the planar scene
	m_pPlanarScene->Create(width, height);

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	m_pHUD->LoadFont("resources\\fonts\\digital-7mono.ttf", 48); // http://files.all-free-download.com/downloadfiles/font/digital_7_font_4073.zip
	m_pHUD->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	m_pSubmarine->Load("resources\\models\\Submarine\\submarine.obj"); //Downloaded from https://free3d.com/es/modelo-3d/mini-submarine-725226.html
	m_pRockA->Load("resources\\models\\Rocks01\\RockA\\rocks_A_model.obj"); //Downloaded from https://free3d.com/es/modelo-3d/rocks--43124.html
	m_pRockB->Load("resources\\models\\Rocks01\\RockB\\rocks_B_model.obj"); //Downloaded from https://free3d.com/es/modelo-3d/rocks--43124.html
	m_pRockC->Load("resources\\models\\Rocks01\\RockC\\rocks_C_model.obj"); //Downloaded from https://free3d.com/es/modelo-3d/rocks--43124.html
	m_pRockD->Load("resources\\models\\Rocks01\\RockD\\rocks_D_model.obj"); //Downloaded from https://free3d.com/es/modelo-3d/rocks--43124.html
	m_pRockE->Load("resources\\models\\Rocks01\\RockE\\rocks_E_model.obj"); //Downloaded from https://free3d.com/es/modelo-3d/rocks--43124.html

	glEnable(GL_CULL_FACE);

	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadObjectSound("resources\\Audio\\submarine-rotor.wav");					// Royalty free sound from freesound.org https://freesound.org/people/urbanmatter/sounds/269705/
	//m_pAudio->LoadSoundSource("resources\\Audio\\sonar-sweep-beep.wav");					// Royalty free sound from freesound.org https://freesound.org/people/SamsterBirdies/sounds/371178/
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-BlueMist.mp3");	// Royalty free music from http://www.nosoapradio.us/ (https://drive.google.com/open?id=0Bw4MH6EXU9vTQzlNQk9qU2dTc0U)
	m_pAudio->PlayMusicStream();
	m_pAudio->PlaySoundSource();
	m_pAudio->PlayObjectSound();
	
	// Create path 
	m_pPath->CreatePath("resources\\textures\\", "SandCOLOR.jpg"); //Texture from: https://3dtextures.me/2017/03/23/sand-002/
	m_pPath->CreateWalls("resources\\textures\\", "Cliff_Rock_001_basecolor.jpg"); //Texture from: https://3dtextures.me/2019/10/23/cliff-rock-001/ 
	m_pPath->CreateWater("resources\\textures\\", "water2.png"); //Texture edited from Moodle
	m_pPath->CreateCentreline();
	m_pPath->CreateOffsetCurves();
	m_pPath->CreateTrack();
	m_pPath->CreateOffsetUpCurves();
	m_pPath->CreateRightWall();
	m_pPath->CreateLeftWall();
	m_pPath->CreateUpTrack();


	//Create first torus
	m_pTorus->Create("resources\\textures\\Greeble_Techno_002_basecolor.jpg", 50, 50, 20, 2);
	//Set torus position and orientation
	glm::vec3 p;
	glm::vec3 up;
	m_pPath->Sample(m_torusDistance, p, up);

	glm::vec3 pNext;
	glm::vec3 upNext;
	m_pPath->Sample(m_torusDistance + 1.0f, pNext, upNext);

	//Normalised tangent vector T that points from p to pNext
	glm::vec3 t = glm::normalize(glm::vec3(pNext.x - p.x, pNext.y - p.y, pNext.z - p.z));
	//N = T x y. Replace y for interpolated up vector
	glm::vec3 n = glm::normalize(glm::cross(t, up));
	//B = N x T
	glm::vec3 b = glm::normalize(glm::cross(n, t));

	m_torusOrientation = glm::mat4(glm::mat3(n, b, t));
	m_torusPosition = glm::vec3(p.x, p.y + 15, p.z) + n * -5.0f;

	//Create second torus
	m_pTorus2->Create("resources\\textures\\Greeble_Techno_002_basecolor.jpg", 50, 50, 20, 2);
	//Set torus position and orientation
	m_pPath->Sample(m_torusDistance2, p, up);
	m_pPath->Sample(m_torusDistance2 + 1.0f, pNext, upNext);

	//Normalised tangent vector T that points from p to pNext
	t = glm::normalize(glm::vec3(pNext.x - p.x, pNext.y - p.y, pNext.z - p.z));
	//N = T x y. Replace y for interpolated up vector
	n = glm::normalize(glm::cross(t, up));
	//B = N x T
	b = glm::normalize(glm::cross(n, t));

	m_torusOrientation2 = glm::mat4(glm::mat3(n, b, t));
	m_torusPosition2 = glm::vec3(p.x, p.y + 15, p.z);
}

// Render method runs repeatedly in a loop
void Game::Render()
{
	//Bind the FBO to render to texture
	m_pFBO->Bind(true);
	//Render the scene as usual
	RenderScene(0);
	//Unbind the FBO to render to the window
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Bind the top view FBO to render to texture
	m_pFBOTopView->Bind(true);
	//Render the scene with top view perspective 
	RenderSceneTopView();
	//Unbind the FBO to render to the window
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Render the scene, but include a object textured with the texture from the fbo
	RenderScene(1);

	// Draw the 2D graphics after the 3D graphics
	//Display HUD
	DisplayHUD();
	//Display Framerate
	DisplayFrameRate();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());
}

//Render scene with top view perspective to texture 
void Game::RenderSceneTopView() {

	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the top view shader program 
	CShaderProgram *pTopViewProgram = (*m_pShaderPrograms)[3];
	pTopViewProgram->UseProgram();
	pTopViewProgram->SetUniform("sampler0", 0);

	// Set the projection matrix
	pTopViewProgram->SetUniform("matrices.projMatrix", m_pCameraTopView->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCameraTopView->GetPosition(), m_pCameraTopView->GetView(), m_pCameraTopView->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCameraTopView->ComputeNormalMatrix(viewMatrix);

	// Render the submarine 
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(m_submarinePosition);
	modelViewMatrixStack *= m_submarineOrientation;
	modelViewMatrixStack.Scale(10.0f);
	pTopViewProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTopViewProgram->SetUniform("matrices.normalMatrix", m_pCameraTopView->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	pTopViewProgram->SetUniform("renderSubmarine", true);
	m_pSubmarine->Render();
	pTopViewProgram->SetUniform("renderSubmarine", false);
	modelViewMatrixStack.Pop();

	//Render the path
	modelViewMatrixStack.Push();
	pTopViewProgram->SetUniform("renderPath", true);
	pTopViewProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTopViewProgram->SetUniform("matrices.normalMatrix", m_pCameraTopView->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPath->RenderTrack();
	pTopViewProgram->SetUniform("renderPath", false);
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, -200.0f, 1000.0f));
	modelViewMatrixStack.Scale(3.0f);
	pTopViewProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pTopViewProgram->SetUniform("matrices.normalMatrix", m_pCameraTopView->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();
	glDisable(GL_BLEND);

}

// Render scene to texture
void Game::RenderScene(int pass) 
{
	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10; 
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);
	

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	// Set lights and materials in main shader program
	if (m_lightOn) {
		pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	}
	else {
		// Turn on diffuse + specular materials
		pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance
		pMainProgram->SetUniform("material1.shininess", 0.5f);		// Shininess material property
	}
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("lights[0].position", (viewMatrix*glm::vec4(m_submarinePosition.x, m_submarinePosition.y, m_submarinePosition.z, 1.0f))); // Light position in eye coordinates
	pMainProgram->SetUniform("lights[0].La", glm::vec3(0.59f, 0.52f, 1.0f)); //violet
	pMainProgram->SetUniform("lights[0].Ld", glm::vec3(1.0f, 1.0f, 0.0f)); //yellow
	pMainProgram->SetUniform("lights[0].Ls", glm::vec3(1.0f, 1.0f, 0.0f)); //yellow
	pMainProgram->SetUniform("lights[0].direction", glm::normalize(viewNormalMatrix*glm::normalize(glm::vec3(m_submarineOrientation[0][0], m_submarineOrientation[0][1], m_submarineOrientation[0][2]))));
	pMainProgram->SetUniform("lights[0].exponent", 20.0f);
	pMainProgram->SetUniform("lights[0].cutoff", 20.0f);
	pMainProgram->SetUniform("lights[1].position", (viewMatrix*glm::vec4(m_torusPosition.x, m_torusPosition.y-10, m_torusPosition.z, 1.0f))); // Light position in eye coordinates
	pMainProgram->SetUniform("lights[1].La", glm::vec3(0.59f, 0.52f, 1.0f)); //violet 
	pMainProgram->SetUniform("lights[1].Ld", glm::vec3(1.0f, 0.25f, 0.0f)); //orange
	pMainProgram->SetUniform("lights[1].Ls", glm::vec3(1.0f, 0.25f, 0.0f)); //orange
	pMainProgram->SetUniform("lights[1].direction", glm::normalize(viewNormalMatrix*glm::normalize(glm::vec3(m_torusOrientation[2][0], m_torusOrientation[2][1], m_torusOrientation[2][2]))));
	pMainProgram->SetUniform("lights[1].exponent", 10.0f);
	pMainProgram->SetUniform("lights[1].cutoff", 20.0f);
	pMainProgram->SetUniform("lights[2].position", (viewMatrix*glm::vec4(m_torusPosition2.x, m_torusPosition2.y - 10, m_torusPosition2.z, 1.0f))); // Light position in eye coordinates
	pMainProgram->SetUniform("lights[2].La", glm::vec3(0.59f, 0.52f, 1.0f)); //violet 
	pMainProgram->SetUniform("lights[2].Ld", glm::vec3(1.0f, 0.25f, 0.0f)); //orange
	pMainProgram->SetUniform("lights[2].Ls", glm::vec3(1.0f, 0.25f, 0.0f)); //orange
	pMainProgram->SetUniform("lights[2].direction", glm::normalize(viewNormalMatrix*glm::normalize(glm::vec3(m_torusOrientation2[2][0], m_torusOrientation2[2][1], m_torusOrientation2[2][2]))));
	pMainProgram->SetUniform("lights[2].exponent", 10.0f);
	pMainProgram->SetUniform("lights[2].cutoff", 20.0f);
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
	//Set the day/night mode
	pMainProgram->SetUniform("dayMode", m_lightOn);		
	//Set elapsed time
	std::chrono::duration<float> elapsedTime = std::chrono::system_clock::now() - m_startTime;
	pMainProgram->SetUniform("time", elapsedTime.count());

	if (pass == 1) {
		// Use the blur shader program 
		CShaderProgram *pBlurProgram = (*m_pShaderPrograms)[2];
		pBlurProgram->UseProgram();
		pBlurProgram->SetUniform("sampler0", 0);

		pBlurProgram->SetUniform("renderBlur", false);

		// Render quad with the minimap in 2D mode
		modelViewMatrixStack.Push();
		modelViewMatrixStack.SetIdentity();
		modelViewMatrixStack.Translate(glm::vec3(600.0f, 0.0f, 0.0f));
		modelViewMatrixStack.Scale(0.25f);
		pBlurProgram->SetUniform("renderMap", true);
		pBlurProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pBlurProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		m_pPlanarScene->RenderScene(m_pFBOTopView);
		pBlurProgram->SetUniform("renderMap", false);
		modelViewMatrixStack.Pop();


		if(m_viewMode==first || m_viewMode == third){
			pBlurProgram->SetUniform("speed", m_submarineSpeed);
			pBlurProgram->SetUniform("renderBlur", true);
		}

		// Render quad in 2D mode
		modelViewMatrixStack.Push();
		modelViewMatrixStack.SetIdentity();
		pBlurProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pBlurProgram->SetUniform("matrices.projMatrix",m_pCamera->GetOrthographicProjectionMatrix());
		m_pPlanarScene->RenderScene(m_pFBO);
		modelViewMatrixStack.Pop();

	}
	else {

		// Render the skybox and terrain with full ambient reflectance 
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
		modelViewMatrixStack.Pop();

		if (!m_lightOn) {
			pMainProgram->SetUniform("material1.shininess", 0.5f);		// Shininess material property
		}

		// Turn on diffuse + specular materials
		pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

		// Render the submarine 
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_submarinePosition);
		modelViewMatrixStack *= m_submarineOrientation;
		modelViewMatrixStack.Scale(0.4f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pSubmarine->Render();
		modelViewMatrixStack.Pop();

		// Render the rocks 
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Scale(0.16f);
		pMainProgram->SetUniform("renderRockA", true);
		pMainProgram->SetUniform("rockScale", 0.16f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pRockA->RenderInstance(10);
		pMainProgram->SetUniform("renderRockA", false);
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Scale(0.16f);
		pMainProgram->SetUniform("renderRockB", true);
		pMainProgram->SetUniform("rockScale", 0.16f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		//m_pRockB->RenderInstance(10);
		pMainProgram->SetUniform("renderRockB", false);
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Scale(0.16f);
		pMainProgram->SetUniform("renderRockC", true);
		pMainProgram->SetUniform("rockScale", 0.16f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pRockC->RenderInstance(10);
		pMainProgram->SetUniform("renderRockC", false);
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Scale(0.16f);
		pMainProgram->SetUniform("renderRockD", true);
		pMainProgram->SetUniform("rockScale", 0.16f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pRockD->RenderInstance(10);
		pMainProgram->SetUniform("renderRockD", false);
		modelViewMatrixStack.Pop();

		modelViewMatrixStack.Push();
		modelViewMatrixStack.Scale(0.16f);
		pMainProgram->SetUniform("renderRockE", true);
		pMainProgram->SetUniform("rockScale", 0.16f);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pRockE->RenderInstance(10);
		pMainProgram->SetUniform("renderRockE", false);
		modelViewMatrixStack.Pop();

		//Render the cubes
		m_cubeRot += 0.001*(float)m_dt;
		for (int i = 0; i < m_pCubes->size(); i++) {
			if (m_pCubes->at(i)->active) {
				modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(m_pCubesPositions->at(i));
				modelViewMatrixStack.Rotate(glm::vec3(0.0f, 1.0f, 0.0f), m_cubeRot);
				modelViewMatrixStack.Scale(5.0f);
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				pMainProgram->SetUniform("bUseTexture", true);
				(*m_pCubes)[i]->Render();
				modelViewMatrixStack.Pop();
			}
		}

		//Render first torus
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_torusPosition);
		modelViewMatrixStack *= m_torusOrientation;
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pTorus->Render();
		modelViewMatrixStack.Pop();

		//Render second torus
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(m_torusPosition2);
		modelViewMatrixStack *= m_torusOrientation2;
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pTorus2->Render();
		modelViewMatrixStack.Pop();

		//Render the path
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSand", true);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pPath->RenderTrack();
		pMainProgram->SetUniform("renderSand", false);
		modelViewMatrixStack.Pop();

		//Render the water 
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderWater", true);
		pMainProgram->SetUniform("bUseTexture", true);
		// turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPath->RenderUpTrack();
		pMainProgram->SetUniform("renderWater", false);
		modelViewMatrixStack.Pop();

		//Render the walls
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderWalls", true);
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		pMainProgram->SetUniform("bUseTexture", true);
		m_pPath->RenderWalls();
		pMainProgram->SetUniform("renderWalls", false);
		modelViewMatrixStack.Pop();

		//Render the spline
		/**
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("bUseTexture", false);
		// turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//m_pPath->RenderCentreline();
		modelViewMatrixStack.Pop();

		//Render the offset splines
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("bUseTexture", false);
		// turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPath->RenderOffsetCurves();
		modelViewMatrixStack.Pop();

		//Render the offset up splines
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("bUseTexture", false);
		// turn off texturing
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		//m_pPath->RenderOffsetUpCurves();
		modelViewMatrixStack.Pop();
		*/
	}
}

// Update method runs repeatedly with the Render method
void Game::Update() 
{

	//Dicrement the submarine speed if it is not longer moving
	if (m_stop) {
		m_submarineSpeed = m_submarineSpeed / 1.01;
	}

	//If has gone through torus, increment speed
	if (m_torusSpeed) {
		m_torusSpeedTime = m_torusSpeedTime + m_dt;
		if (m_torusSpeedTime < 3000) {
			m_submarineSpeed = 0.21f;
		} else {
			m_submarineSpeed = 0.15f;
			m_torusSpeed = false;
			m_torusSpeedTime = 0.0;
		}
	}

	//Moving the camera 

	//Updates the distance
	m_currentDistance += m_dt * m_submarineSpeed;

	//Moving the submarine 
	glm::vec3 pSub;
	glm::vec3 upSub;
	m_pPath->Sample(m_currentDistance, pSub, upSub);

	//Direction of the submarine
	glm::vec3 pNextSub;
	glm::vec3 upNextSub;
	m_pPath->Sample(m_currentDistance + 1.0f, pNextSub, upNextSub);

	//Normalised tangent vector T that points from p to pNext
	glm::vec3 t = glm::normalize(glm::vec3(pNextSub.x - pSub.x, pNextSub.y - pSub.y, pNextSub.z - pSub.z));
	//N = T x y. Replace y for interpolated up vector
	glm::vec3 n = glm::normalize(glm::cross(t, upSub));
	//B = N x T
	glm::vec3 b = glm::normalize(glm::cross(n, t));
	m_submarineSide = n * m_submarinesSideDif;

	//Collision detection
	float rockCollisionDistance = 15;
	for (int i = 0; i < m_pRocksPositions->size(); i++) {
		glm::vec3 pos = glm::vec3(m_pRocksPositions->at(i).x, m_submarinePosition.y, m_pRocksPositions->at(i).z);
		float d = glm::length(m_submarinePosition - pos);
		if (d < rockCollisionDistance) {
			m_torusSpeed = false;
			m_currentDistance = m_currentDistance - 0.5*glm::length(m_submarinePosition - m_pRocksPositions->at(i));
			//Moving the submarine 
			glm::vec3 pSub;
			glm::vec3 upSub;
			m_pPath->Sample(m_currentDistance, pSub, upSub);

			//Direction of the submarine
			glm::vec3 pNextSub;
			glm::vec3 upNextSub;
			m_pPath->Sample(m_currentDistance + 1.0f, pNextSub, upNextSub);

			//Normalised tangent vector T that points from p to pNext
			glm::vec3 t = glm::normalize(glm::vec3(pNextSub.x - pSub.x, pNextSub.y - pSub.y, pNextSub.z - pSub.z));
			//N = T x y. Replace y for interpolated up vector
			glm::vec3 n = glm::normalize(glm::cross(t, upSub));
			//B = N x T
			glm::vec3 b = glm::normalize(glm::cross(n, t));
			m_submarineSide = n * m_submarinesSideDif;
			m_submarineSpeed = -0.01f;
			m_stop = true;
		}
	}

	m_submarineOrientation = glm::mat4(glm::mat3(t, b , n));
	m_submarinePosition = glm::vec3(pSub.x, pSub.y + 15, pSub.z) + m_submarineSide;
	
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_view = glm::vec3(0.0f);
	glm::vec3 m_upVector = glm::vec3(0.0f);

	switch (m_viewMode)
	{
	case first:
		m_position = m_submarinePosition;
		m_view = glm::vec3(pNextSub.x, pNextSub.y + 15, pNextSub.z) + m_submarineSide;
		m_pCamera->Set(m_position, m_view, upSub);
		break;
	case side:
		m_position = m_submarinePosition - n * 50.f;
		m_pCamera->Set(m_position, m_submarinePosition, upSub);
		break;
	case third:
		m_position = (pSub - t * 80.0f + b * 20.0f) + m_submarineSide;
		m_view = (pSub + b * 20.0f);
		m_upVector = glm::rotate(upSub, m_cameraRotation, t);
		m_pCamera->Set(m_position, m_view, m_upVector);
		break;
	case debugging:
		// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
		m_pCamera->Update(m_dt);
		break;
	}

	//Cube recollected detection
	float cubeCollisionDistance = 10;
	if (m_pPath->CurrentLap(m_currentDistance) < 1) {
		for (int i = 0; i < m_pCubesPositions->size(); i++) {
			float d = glm::length(glm::vec2(m_submarinePosition.x, m_submarinePosition.z) - glm::vec2(m_pCubesPositions->at(i).x, m_pCubesPositions->at(i).z));
			if (d < cubeCollisionDistance && m_pCubes->at(i)->active) {
				m_pCubes->at(i)->active = false;
				m_collectedCubes++;
			}
		}
	}

	//Going through tori detection
	if (glm::length(glm::vec2(m_submarinePosition.x, m_submarinePosition.z) - glm::vec2(m_torusPosition.x, m_torusPosition.z)) < 10.0f) {
		m_torusSpeed = true;
	}
	if (glm::length(glm::vec2(m_submarinePosition.x, m_submarinePosition.z) - glm::vec2(m_torusPosition2.x, m_torusPosition2.z)) < 10.0f) {
		m_torusSpeed = true;
	}
	//m_pCamera->Set(glm::vec3(300.0, 3000.0, 1200.0), glm::vec3(300.0, 0.0, 1200.0), glm::vec3(1.0, 0.0, 0.0));
	//DSP and audio programming coursework:
	/**
	//Submarine position and orientation
	m_t += m_submarineVel * (float)m_dt;
	float r = 100.0f;  
	glm::vec3 x = glm::vec3(1, 0, 0);  
	glm::vec3 y = glm::vec3(0, 1, 0);  
	glm::vec3 z = glm::vec3(0, 0, 1); 
	//Sets submarine position
	m_submarinePosition = r * cos(m_t) * x + 15.0f * y + r * sin(m_t) * z;
	glm::vec3 T = glm::normalize(-r * sin(m_t) * x + r * cos(m_t) * z); 
	glm::vec3 N = glm::normalize(glm::cross(T, y)); 
	glm::vec3 B = glm::normalize(glm::cross(N, T));
	//Sets submarine orientation
	m_submarineOrientation = glm::mat4(glm::mat3(T, B, N));
	//Sets position and velocity vectors for the submarine sound source
	m_pSubmarineSoundSource->SetPosition(m_submarinePosition);
	m_pSubmarineSoundSource->SetVelocity(glm::vec3(m_t, 0, m_t));

	//Updates the module sound source 
	m_pSoundSource->Update(m_dt);
	*/
	//Updates the audio object
	m_pAudio->Update(m_filterControl, m_pSoundSource, m_pSubmarineSoundSource, m_submarineSpeed, m_pCamera);
}



void Game::DisplayFrameRate()
{

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
    {
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
    }

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
	}
}

void Game::DisplayHUD()
{

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;
	int width = dimensions.right - dimensions.left;


	// Use the font shader program and render the text
	fontProgram->UseProgram();
	glDisable(GL_DEPTH_TEST);
	fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
	fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
	fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pHUD->Render(20, height - 500, 32, "CHESTS");
	m_pHUD->Render(110, height - 500, 48, "%d", m_collectedCubes);
	m_pHUD->Render(132, height - 500, 32, "/%d", m_pCubes->size());
	m_pHUD->Render(20, height - 540, 32, "TIME");
	if(m_start){
		if (m_pPath->CurrentLap(m_currentDistance) < 1) {
			m_timePlayed += m_dt;
		}
		else {
			m_pHUD->Render(width / 2 - 160, height - 80, 64, "POINTS:%d", (int)glm::abs(400 - m_timePlayed/1000) + 100 * m_collectedCubes);
		}
		m_pHUD->Render(90, height - 540, 48, "%02d:%02d", (int)((m_timePlayed / 1000) / 60), (int)(m_timePlayed / 1000) % 60);
	}
	else {
		m_pHUD->Render(90, height - 540, 48, "00:00");
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}

WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_appActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {

	
	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_appActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_appActive = false;
				break;
		}
		break;
	}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch(w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case 'L':
			m_lightOn = !m_lightOn;
			break;
		case '1':
			m_viewMode = first;
			break;
		case '2':
			m_viewMode = side;
			break;
		case '3':
			m_viewMode = third;
			break;
		case '4':
			m_viewMode = debugging;
			break;
		case 'W':
			if (!m_start) {
				m_start = true;
			}
			m_stop = false;
			if (!m_torusSpeed) {
				if (m_submarineSpeed <= 0) {
					m_submarineSpeed = 0.01;
				}
				else if (m_submarineSpeed < 0.15) {
					m_submarineSpeed = m_submarineSpeed * 1.05;
				}
			}
			break;
		case 'S':
			m_stop = false;
			if (m_submarineSpeed > 0) {
				m_submarineSpeed = m_submarineSpeed / 1.05;
			}
			break;
		case 'D':
			//m_cameraRotation = m_cameraRotation + m_dt * 0.01f;
			if(m_pPath->w/4.5 > m_submarinesSideDif)
				m_submarinesSideDif = m_dt * glm::abs(m_submarineSpeed) + m_submarinesSideDif;
			break;
		case 'A':
			//m_cameraRotation = m_cameraRotation  - m_dt * 0.01f;
			if (-m_pPath->w / 4.5 < m_submarinesSideDif)
				m_submarinesSideDif = -m_dt * glm::abs(m_submarineSpeed) + m_submarinesSideDif;
			break;
		case VK_SPACE:
			m_stop = true;
			m_torusSpeed = false;
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() 
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
