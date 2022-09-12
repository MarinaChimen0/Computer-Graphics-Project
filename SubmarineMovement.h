#pragma once

#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtc/matrix_transform.hpp"

class SubmarineMovement {
public:
	SubmarineMovement();										// Constructor - sets default values for submarine position, viewvector, upvector, and speed
	~SubmarineMovement();										// Destructor

	glm::vec3 GetPosition() const;					// Gets the position of the submarine centre of projection
	glm::vec3 GetView() const;						// Gets the position of the submarine view point
	glm::vec3 GetUpVector() const;					// Gets the submarine up vector
	glm::vec3 GetStrafeVector() const;				// Gets the submarine strafe vector
	glm::mat4* GetPerspectiveProjectionMatrix();	// Gets the submarine perspective projection matrix
	glm::mat4* GetOrthographicProjectionMatrix();	// Gets the submarine orthographic projection matrix
	glm::mat4 GetViewMatrix();						// Gets the submarine view matrix - note this is not stored in the class but returned using glm::lookAt() in GetViewMatrix()

	// Set the submarine position, viewpoint, and up vector
	void Set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &upVector);

	// Rotate the submarine viewpoint -- this effectively rotates the submarine
	void RotateViewPoint(float angle, const glm::vec3 &viewPoint);

	// Respond to mouse movement to rotate the submarine
	void SetViewByMouse();

	// Respond to keyboard presses on arrow keys to translate the submarine
	void TranslateByKeyboard(double dt);

	// Strafe the submarine (move it side to side)
	void Strafe(double direction);

	// Advance the submarine (move it forward or backward)
	void Advance(double direction);

	// Update the submarine
	void Update(double dt);

	// Set the projection matrices
	void SetPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane);
	void SetOrthographicProjectionMatrix(int width, int height);

	glm::mat3 ComputeNormalMatrix(const glm::mat4 &modelViewMatrix);

private:
	glm::vec3 m_position;			// The position of the submarine's centre of projection
	glm::vec3 m_view;				// The submarine's viewpoint (point where the submarine is looking)
	glm::vec3 m_upVector;			// The submarine's up vector
	glm::vec3 m_strafeVector;		// The submarine's strafe vector

	float m_speed;					// How fast the submarine moves

	glm::mat4 m_perspectiveProjectionMatrix;		// Perspective projection matrix
	glm::mat4 m_orthographicProjectionMatrix;		// Orthographic projection matrix
};
