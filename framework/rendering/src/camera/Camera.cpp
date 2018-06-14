#include <GL/freeglut.h>
#include "rtr/camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#ifndef PI
#define PI 3.141592f
#endif

#define CAMERA_ROTATE 1
#define CAMERA_MOVE 2

using namespace glm;

Camera::Camera(float ratio, vec3 camPos)
{
	mOldX = 0;
	mOldY = 0;
	mMotionState = CAMERA_ROTATE;										
	mTheta = 2.0;								
	mPhi = 4.0;									
	mSpeed = 0.0f;														
	mMotionStep = 0.001f;											
	mThetaStep = 0.001f;
	mPhiStep = 0.003f;
	//mPosition = vec3(70.0f, 64.0f, 70.0f);	
	mPosition = camPos;

	mProjectionMatrix = perspective(45.0f, ratio, 0.01f, 10000.0f);

}

Camera::~Camera()
{

}


void Camera::mouseButton(int button, int state, int x, int y)
{

	mMove = true;
	
	mOldX = x;
	mOldY = y;

	// Left mouse button: turn camera
	if (button == GLUT_LEFT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{
			mMotionState = CAMERA_ROTATE;
		}
	}

	// right mouse button: move camera
	else if (button == GLUT_RIGHT_BUTTON) 
	{
		if (state == GLUT_DOWN) 
		{
			mMotionState = CAMERA_MOVE;
		}
	}

}

void Camera::mouseMove(int x, int y)
{
	
	int deltaX = x - mOldX;
	int deltaY = y - mOldY;

	if (mMotionState == CAMERA_ROTATE) 
	{
		mTheta += mThetaStep * static_cast<float>(deltaY);

		if (mTheta < mThetaStep) 
			mTheta = mThetaStep;
		else if (mTheta > PI - mThetaStep) 
			mTheta = PI - mThetaStep;

		mPhi += mPhiStep * static_cast<float>(deltaX);

		if (mPhi < 0.0f) 
			mPhi += 2.0f*PI;
		else if (mPhi > 2.0f*PI) 
			mPhi -= 2.0f*PI;
	}
	else if (mMotionState == CAMERA_MOVE) 
	{
		mSpeed -= mMotionStep * static_cast<float>(deltaY);
	}

	mOldX = x;
	mOldY = y;
}

void Camera::update()
{
		mDirection.x = sin(mTheta) * cos(mPhi);
		mDirection.y = cos(mTheta);
		mDirection.z = sin(mTheta) * sin(mPhi);

		mPosition += mSpeed * mDirection;

		mViewMatrix = lookAt(mPosition, mPosition + mDirection, vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& Camera::getViewMatrix() const
{
	return mViewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
}