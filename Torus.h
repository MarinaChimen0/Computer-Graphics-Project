#pragma once 

#include "Common.h" 
#include "Texture.h" 
#include "VertexBufferObjectIndexed.h"

// Class for generating a torus 
class CTorus
{
public:
	CTorus();
	~CTorus();
	void Create(string filename, int circleSegments, int bodySegments, float circleRadius, float bodyRadius);
	void Render();
	void Release();

private:
	GLuint m_vao;
	CVertexBufferObjectIndexed m_vbo;
	CTexture m_texture;

	int numIndex; 
	int primitiveRestartIndex; 
};