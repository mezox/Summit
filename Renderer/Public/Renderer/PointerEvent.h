#pragma once

#include <cstdint>

enum class MouseEventType
{
    Invalid,
    Move,
    LeftDrag,
    RightDrag,
    LeftPress,
    LeftRelease,
    RightPress,
    RightRelease
};

struct PointerEvent
{
    MouseEventType type;
    float x;
    float y;
    float offsetX;
    float offsetY;
};
