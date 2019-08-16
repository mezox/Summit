#pragma once

#include "Math.h"
#include "Matrix4.h"

#include "Math/Math.h"

namespace Summit
{
    class Camera
    {
    public:
        Camera() = default;
        
        void Update();
        
        void SetPerspectiveProjection(float fovY, float width, float height, float near, float far);
        void SetOrtographicProjection(float left, float right, float bottom, float top, float near, float far);
        
        void SetPosition(const Math::Float3& position) { mPosition = position; }
        void SetRotation(const Math::Float3& rotation) { mRotation = rotation; }
        
        const Math::Float3& GetPosition() const { return mPosition; }
        const Math::Float3& GetRotation() const { return mRotation; }
        const Math::Float4x4& GetProjectionMatrix() const { return mProjectionMatrix; }
        const Math::Float4x4& GetViewMatrix() const { return mViewMatrix; }
        
        void RotateX(float angle) { mRotation.x += angle; }
        void RotateY(float angle) { mRotation.y += angle; }
        

    private:
        Math::Float3 mPosition;
        Math::Float3 mRotation;
        
        float mNearPlane{ 0.0f };
        float mFarPlane{ 0.0f };
        float mFovX{ 0.0f };
        float mFovY{ 0.0f };
        float mAspectRatio{ 0.0f };
        float mViewportWidth{ 0.0f };
        float mViewportHeight{ 0.0f };

        float mLeft{ 0.0f };
        float mRight{ 0.0f };
        float mBottom{ 0.0f };
        float mTop{ 0.0f };

        bool mIsPerspective{ false };
        
    private:
        Math::Float4x4 mViewMatrix;
        Math::Float4x4 mProjectionMatrix;
    };
}
