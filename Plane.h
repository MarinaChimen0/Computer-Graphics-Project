#pragma once

#include "Texture.h"
#include "VertexBufferObject.h"
#include "FrameBufferObject.h"

// Class for generating a xz plane of a given size
class CPlane
{
public:
	CPlane();
	~CPlane();
	void Create(string sDirectory, string sFilename, float fWidth, float fHeight, float fTextureRepeat);
	void Create(float width, float height);
	void Render();
	void RenderScene(CFrameBufferObject* fbo);
	void Release();
private:
	UINT m_vao;
	CVertexBufferObject m_vbo;
	CTexture m_texture;
	string m_directory;
	string m_filename;
	float m_width;
	float m_height;
};