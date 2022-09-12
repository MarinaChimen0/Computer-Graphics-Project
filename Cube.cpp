#include "Cube.h" 

CCube::CCube() {} 

CCube::~CCube() {    Release(); } 

void CCube::Create(string filename) {   

	m_texture.Load(filename);    
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);    
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);    
	glGenVertexArrays(1, &m_vao);  
	glBindVertexArray(m_vao);  
	m_vbo.Create();
	m_vbo.Bind();    

	// Write the code to add interleaved vertex attributes to the VBO 

	//front face
	glm::vec3 v0 = glm::vec3(-1, -1, 1);
	glm::vec3 v1 = glm::vec3(1, -1, 1);
	glm::vec3 v2 = glm::vec3(1, 1, 1);
	glm::vec3 v3 = glm::vec3(-1, 1, 1);
	glm::vec3 n0 = glm::vec3(0, 0, 1);
	glm::vec2 t0 = glm::vec2(0, 0);
	glm::vec2 t1 = glm::vec2(1, 0);
	glm::vec2 t2 = glm::vec2(1, 1);
	glm::vec2 t3 = glm::vec2(0, 1);

	m_vbo.AddData(&v0, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n0, sizeof(glm::vec3));
	m_vbo.AddData(&v1, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n0, sizeof(glm::vec3));
	m_vbo.AddData(&v3, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n0, sizeof(glm::vec3));
	m_vbo.AddData(&v2, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n0, sizeof(glm::vec3));

	//back face
	glm::vec3 v4 = glm::vec3(1, 1, -1);
	glm::vec3 v5 = glm::vec3(1, -1, -1);
	glm::vec3 v6 = glm::vec3(-1, 1, -1);
	glm::vec3 v7 = glm::vec3(-1, -1, -1);
	glm::vec3 n1 = glm::vec3(0, 0, -1);

	m_vbo.AddData(&v4, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));
	m_vbo.AddData(&v5, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));
	m_vbo.AddData(&v6, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));
	m_vbo.AddData(&v7, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n1, sizeof(glm::vec3));

	//right face
	glm::vec3 v8 = glm::vec3(1, 1, 1);
	glm::vec3 v9 = glm::vec3(1, -1, 1);
	glm::vec3 v10 = glm::vec3(1, 1, -1);
	glm::vec3 v11 = glm::vec3(1, -1, -1);
	glm::vec3 n2 = glm::vec3(1, 0, 0);

	m_vbo.AddData(&v8, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));
	m_vbo.AddData(&v9, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));
	m_vbo.AddData(&v10, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));
	m_vbo.AddData(&v11, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n2, sizeof(glm::vec3));

	//left face
	glm::vec3 v12 = glm::vec3(-1, 1, -1);
	glm::vec3 v13 = glm::vec3(-1, -1, -1);
	glm::vec3 v14 = glm::vec3(-1, 1, 1);
	glm::vec3 v15 = glm::vec3(-1, -1, 1);
	glm::vec3 n3 = glm::vec3(-1, 0, 0);

	m_vbo.AddData(&v12, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));
	m_vbo.AddData(&v13, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));
	m_vbo.AddData(&v14, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));
	m_vbo.AddData(&v15, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n3, sizeof(glm::vec3));

	//up face
	glm::vec3 v16 = glm::vec3(-1, 1, -1);
	glm::vec3 v17 = glm::vec3(-1, 1, 1);
	glm::vec3 v18 = glm::vec3(1, 1, -1);
	glm::vec3 v19 = glm::vec3(1, 1, 1);
	glm::vec3 n4 = glm::vec3(0, 1, 0);

	m_vbo.AddData(&v16, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));
	m_vbo.AddData(&v17, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));
	m_vbo.AddData(&v18, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));
	m_vbo.AddData(&v19, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n4, sizeof(glm::vec3));

	//down face
	glm::vec3 v20 = glm::vec3(-1, -1, 1);
	glm::vec3 v21 = glm::vec3(-1, -1, -1);
	glm::vec3 v22 = glm::vec3(1, -1, 1);
	glm::vec3 v23 = glm::vec3(1, -1, -1);
	glm::vec3 n5 = glm::vec3(0, -1, 0);

	m_vbo.AddData(&v20, sizeof(glm::vec3));
	m_vbo.AddData(&t3, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));
	m_vbo.AddData(&v21, sizeof(glm::vec3));
	m_vbo.AddData(&t0, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));
	m_vbo.AddData(&v22, sizeof(glm::vec3));
	m_vbo.AddData(&t2, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));
	m_vbo.AddData(&v23, sizeof(glm::vec3));
	m_vbo.AddData(&t1, sizeof(glm::vec2));
	m_vbo.AddData(&n5, sizeof(glm::vec3));


	// Upload data to GPU    
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);    
	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);    
	// Vertex positions
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);   
	// Texture coordinates
	glEnableVertexAttribArray(1);   
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));    
	// Normal vectors    
	glEnableVertexAttribArray(2);  
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) + sizeof(glm::vec2))); 
} 

void CCube::Render() {   
	glBindVertexArray(m_vao);  
	m_texture.Bind();   
	// Call glDrawArrays to render each side 
	for (int i = 0; i < 6; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
} 

void CCube::Release() {   
	m_texture.Release(); 
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release(); 
} 