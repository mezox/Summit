#include <Renderer/Camera.h>
#include <Renderer/Math/Math.h>

#include <iostream>

using namespace Math;

namespace Summit
{
    void Camera::SetPerspectiveProjection(float fovY, float width, float height, float near, float far)
    {
        mFovY = fovY;
        mViewportWidth = width;
        mViewportHeight = height;
        mNearPlane = near;
        mFarPlane = far;

        mIsPerspective = true;
    }

    void Camera::SetOrtographicProjection(float left, float right, float bottom, float top, float near, float far)
    {
        mNearPlane = near;
        mFarPlane = far;
        mLeft = left;
        mRight = right;
        mBottom = bottom;
        mTop = top;

        mIsPerspective = false;
    }
    
    void Camera::Update()
    {
        if(mIsPerspective)
            mProjectionMatrix = Math::MakePerspectiveProjection<float>(DegreesToRadians(mFovY), mViewportWidth / mViewportHeight, mNearPlane, mFarPlane);
        else
            mProjectionMatrix = Math::MakeOrtographicProjection<float>(mLeft, mRight, mBottom, mTop, mNearPlane, mFarPlane);
        
        Math::Quaternion pitch(Float3(1.0f, 0.0f, 0.0f), mRotation.x);
        Math::Quaternion yaw(Float3(0.0f, 1.0f, 0.0f), mRotation.y);
        Math::Quaternion roll(Float3(0.0f, 0.0f, 1.0f), mRotation.z);
        
        Math::Quaternion orientation = pitch * yaw;
        orientation.normalize();
        
        Math::Float4x4 orientationMatrix(orientation);
        Math::Float4x4 translationMatrix = Math::translate(mPosition);

        Math::Float4x4 viewMatrix = translationMatrix * orientationMatrix;
        viewMatrix.make_inverse();

        mViewMatrix = std::move(viewMatrix);
    }
}
