#pragma once

#include "Common.h"
#include "GameWindow.h"
#include <chrono>

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CSoundSource;
class CFrameBufferObject;
class CCatmullRom;
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCube;
class CTorus;

class Game {

enum ViewModes { first, side, third, debugging };

private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void RenderScene(int pass);
	void RenderSceneTopView();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	vector<glm::vec3> *m_pCubesPositions;
	vector<glm::vec3> *m_pRocksPositions;
	CTorus *m_pTorus;
	CTorus *m_pTorus2;
	vector<CCube *> *m_pCubes;
	CFrameBufferObject *m_pFBO;
	CFrameBufferObject *m_pFBOTopView;
	CCatmullRom *m_pPath;
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	CCamera *m_pCameraTopView;
	vector <CShaderProgram *> *m_pShaderPrograms;
	CPlane *m_pPlanarTerrain;
	CPlane *m_pPlanarScene;
	CFreeTypeFont *m_pFtFont;
	CFreeTypeFont *m_pHUD;
	COpenAssetImportMesh *m_pBarrelMesh;
	COpenAssetImportMesh *m_pHorseMesh;
	COpenAssetImportMesh *m_pModule;
	COpenAssetImportMesh *m_pSubmarine;
	COpenAssetImportMesh *m_pRockA;
	COpenAssetImportMesh *m_pRockB;
	COpenAssetImportMesh *m_pRockC;
	COpenAssetImportMesh *m_pRockD;
	COpenAssetImportMesh *m_pRockE;
	COpenAssetImportMesh *m_pBarrels;
	CSphere *m_pSphere;
	CHighResolutionTimer *m_pHighResolutionTimer;
	CAudio *m_pAudio;
	CSoundSource *m_pSoundSource;
	CSoundSource *m_pSubmarineSoundSource;
	ViewModes m_viewMode;

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
	float m_currentDistance;
	float m_cameraSpeed;
	float m_cameraRotation;
	float m_filterControl;
	//float m_t;
	int m_collectedCubes;
	float m_cubeRot;
	float m_submarinesSideDif;
	float m_submarineSpeed;
	glm::vec3 m_submarinePosition;
	glm::vec3 m_submarineSide;
	glm::vec3 m_submarineUp;
	glm::mat4 m_submarineOrientation;
	float m_torusDistance;
	glm::vec3 m_torusPosition;
	glm::mat4 m_torusOrientation;
	float m_torusDistance2;
	glm::vec3 m_torusPosition2;
	glm::mat4 m_torusOrientation2;
	bool m_torusSpeed;
	double m_torusSpeedTime;
	bool m_stop;
	float m_lightOn;
	std::chrono::system_clock::time_point m_startTime;
	bool m_start; 
	double m_timePlayed;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	void DisplayHUD();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
};
