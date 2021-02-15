#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 25.f;
	Vector3 view = (target - position).Normalize();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalize();

	if (Application::IsKeyPressed('W'))
	{
		Mtx44 rotation;
		rotation.SetToRotation((-CAMERA_SPEED * dt), right.x, right.y, right.z);
		position = rotation * position;
	}
	else if (Application::IsKeyPressed('S'))
	{
		Mtx44 rotation;
		rotation.SetToRotation(CAMERA_SPEED * dt, right.x, right.y, right.z);
		position = rotation * position;
	}
	if (Application::IsKeyPressed('A'))
	{
		Mtx44 rotation;
		rotation.SetToRotation((-CAMERA_SPEED * dt), 0, 1, 0);
		position = rotation * position;
		//up = rotation * up;
	}
	else if (Application::IsKeyPressed('D'))
	{
		Mtx44 rotation;
		rotation.SetToRotation(CAMERA_SPEED * dt, 0, 1, 0);
		position = rotation * position;
		//up = rotation * up;
	}
	if (Application::IsKeyPressed('M'))
	{
		position -= view * dt * 10.f;
	}
	else if (Application::IsKeyPressed('N'))
	{
		position += view * dt * 10.f;
	}
}