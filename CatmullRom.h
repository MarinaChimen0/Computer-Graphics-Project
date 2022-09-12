#pragma once
#include "Common.h"
#include "vertexBufferObject.h"
#include "vertexBufferObjectIndexed.h"
#include "Texture.h"


class CCatmullRom
{
public:
	float w; //Width of the path

	CCatmullRom();
	~CCatmullRom();

	void CreatePath(string a_sDirectory, string a_sFilename);
	void CreateWalls(string a_sDirectory, string a_sFilename);
	void CreateWater(string a_sDirectory, string a_sFilename);

	void CreateCentreline();
	void RenderCentreline();

	void CreateOffsetCurves();
	void RenderOffsetCurves();

	void CreateOffsetUpCurves();
	void RenderOffsetUpCurves();

	void CreateTrack();
	void RenderTrack();

	void CreateUpTrack();
	void RenderUpTrack();

	void CreateRightWall();
	void CreateLeftWall();
	void RenderWalls();

	int CurrentLap(float d); // Return the currvent lap (starting from 0) based on distance along the control curve.

	bool Sample(float d, glm::vec3 &p, glm::vec3 &up); // Return a point on the centreline based on a certain distance along the control curve.

private:

	void SetControlPoints();
	void ComputeLengthsAlongControlPoints();
	void UniformlySampleControlPoints(int numSamples);
	glm::vec3 Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t);


	vector<float> m_distances;
	CTexture m_texture;
	CTexture m_wallsTexture;
	CTexture m_waterTexture;

	GLuint m_vaoCentreline;
	GLuint m_vaoLeftOffsetCurve;
	GLuint m_vaoRightOffsetCurve;
	GLuint m_vaoUpLeftOffsetCurve;
	GLuint m_vaoUpRightOffsetCurve;
	GLuint m_vaoTrack;
	vector<GLuint> m_vaoUpTrack;
	GLuint m_vaoRightWall;
	GLuint m_vaoLeftWall;

	vector<glm::vec3> m_controlPoints;		// Control points, which are interpolated to produce the centreline points
	vector<glm::vec3> m_controlUpVectors;	// Control upvectors, which are interpolated to produce the centreline upvectors
	
	vector<glm::vec3> m_centrelinePoints;	// Centreline points
	vector<glm::vec3> m_centrelineUpVectors;// Centreline upvectors

	vector<glm::vec3> m_leftOffsetPoints;	// Left offset curve points
	vector<glm::vec3> m_rightOffsetPoints;	// Right offset curve points

	vector<glm::vec3> m_leftOffsetUpPoints;	// Left offset up curve points
	vector<glm::vec3> m_rightOffsetUpPoints;	// Right offset up curve points
	vector<glm::vec3> m_wallsUpVector; //Up vectors for the walls

	vector < vector<glm::vec3> > m_watertOffsetsPoints;	// Water offset up points
	vector < vector<glm::vec3> > m_leftWatertOffsetsPoints;	// Left offset up water points
	vector < vector<glm::vec3> > m_rightWatertOffsetsPoints; // Right offset up water points

	unsigned int m_vertexCount;				// Number of vertices in the track VBO
	unsigned int m_vertexWaterCount;				// Number of vertices in the water track VBO

	float h; //Height of the walls
};
