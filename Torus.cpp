#include "Torus.h" 

CTorus::CTorus() {}

CTorus::~CTorus() { Release(); }

void CTorus::Create(string filename, int circleSegments, int bodySegments, float circleRadius, float bodyRadius) {

	m_texture.Load(filename);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	m_texture.SetSamplerObjectParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_vbo.Create();
	m_vbo.Bind();

	// Write the code to add interleaved vertex attributes to the VBO - Torus functions taken from https://en.wikipedia.org/wiki/Torus

	// Calculate num of vertices and indices
	int numVertex = (circleSegments + 1)*(bodySegments + 1);
	primitiveRestartIndex = numVertex;
	numIndex = (circleSegments * 2 * (bodySegments + 1)) + circleSegments - 1;

	//Vertex position
	float circleSegmentAngleStep = glm::radians(360.0f / float(circleSegments));
	float bodySegmentAngleStep = glm::radians(360.0f / float(bodySegments));

	//Circle lopp
	float currentCircleSegmentAngle = 0.0f;
	for (unsigned int i = 0; i <= circleSegments; i++)
	{
		// Sine / cosine of circle segment angle
		float sinCircleSegment = sin(currentCircleSegmentAngle);
		float cosCircleSegment = cos(currentCircleSegmentAngle);

		//Body loop
		float currentBodySegmentAngle = 0.0f;
		for (unsigned int j = 0; j <= bodySegments; j++)
		{
			// Calculate sine and cosine of tube segment angle
			float sinBodySegment = sin(currentBodySegmentAngle);
			float cosBodySegment = cos(currentBodySegmentAngle);

			// Calculate vertex position on the surface of torus
			glm::vec3 p = glm::vec3((circleRadius + bodyRadius * cosBodySegment)*cosCircleSegment,
				(circleRadius + bodyRadius * cosBodySegment)*sinCircleSegment, bodyRadius*sinBodySegment);

			m_vbo.AddVertexData(&p, sizeof(glm::vec3));

			// Update current body angle
			currentBodySegmentAngle += bodySegmentAngleStep;
		}

		// Update circle step angle
		currentCircleSegmentAngle += circleSegmentAngleStep;
	}

	//Vertex texture coordinates
	float circleSegmentTextureStep = 2.0f / float(circleSegments);
	float bodySegmentTextureStep = 1.0f / float(bodySegments);

	//Circle loop
	float currentCircleSegmentTexCoord = 0.0f;
	for (unsigned int i = 0; i <= circleSegments; i++)
	{

		//Body loop
		float currentBodySegmentTexCoord = 0.0f;
		for (unsigned int j = 0; j <= bodySegments; j++)
		{
			glm::vec2 s = glm::vec2(currentBodySegmentTexCoord, currentCircleSegmentTexCoord);
			m_vbo.AddVertexData(&s, sizeof(glm::vec2));

			currentBodySegmentTexCoord += bodySegmentTextureStep;
		}

		// Update texture coordinate of circle segment
		currentCircleSegmentTexCoord += circleSegmentTextureStep;
	}

	//Vertex normals

	//Circle loop
	currentCircleSegmentAngle = 0.0f;
	for (unsigned int i = 0; i <= circleSegments; i++)
	{
		// Sine / cosine of circle segment angle
		float sinCircleSegment = sin(currentCircleSegmentAngle);
		float cosCircleSegment = cos(currentCircleSegmentAngle);

		//Body loop
		float currentBodySegmentAngle = 0.0f;
		for (unsigned int j = 0; j <= bodySegments; j++)
		{
			// Calculate sine and cosine of body segment angle
			float sinBodySegment = sin(currentBodySegmentAngle);
			float cosBodySegment = cos(currentBodySegmentAngle);

			glm::vec3 n = glm::vec3(cosCircleSegment*cosBodySegment, sinCircleSegment*cosBodySegment, sinBodySegment);
			m_vbo.AddVertexData(&n, sizeof(glm::vec3));

			// Update current body angle
			currentBodySegmentAngle += bodySegmentAngleStep;
		}

		// Update circle segment angle
		currentCircleSegmentAngle += circleSegmentAngleStep;
	}

	// Add the indices data
	GLuint vertexOffset = 0;
	for (unsigned int i = 0; i < circleSegments; i++)
	{
		for (unsigned int j = 0; j <= bodySegments; j++)
		{
			//Adds one vertex from the current segment and another vertex from the next circle segment
			GLuint currentVertexIndex = vertexOffset;
			m_vbo.AddIndexData(&currentVertexIndex, sizeof(GLuint));
			GLuint nextVertexIndex = vertexOffset + bodySegments + 1;
			m_vbo.AddIndexData(&nextVertexIndex, sizeof(GLuint));
			vertexOffset++;
		}

		// Doesn't restart the primitive, its going to use GL_PRIMITIVE_RESTART
		if (i != circleSegments - 1) {
			m_vbo.AddIndexData(&primitiveRestartIndex, sizeof(GLuint));
		}
	}

	// Upload data to GPU    
	m_vbo.UploadDataToGPU(GL_STATIC_DRAW);

	GLsizei stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(numVertex * sizeof(glm::vec3)));
	// Normal vectors    
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(numVertex * (sizeof(glm::vec3) + sizeof(glm::vec2))));

}

void CTorus::Render() {
	glBindVertexArray(m_vao);
	m_texture.Bind();
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(primitiveRestartIndex);
	glDrawElements(GL_TRIANGLE_STRIP, numIndex, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);
}

void CTorus::Release() {
	m_texture.Release();
	glDeleteVertexArrays(1, &m_vao);
	m_vbo.Release();
}