#pragma once

#include <random>

struct Window_Area
{
    int PLAY_AREA_TOP;
    int PLAY_AREA_LEFT;
    int PLAY_AREA_RIGHT;
    int PLAY_AREA_BOTTOM;

    Window_Area(int top, int left, int right, int bottom)
    : PLAY_AREA_TOP(top), PLAY_AREA_LEFT(left), PLAY_AREA_RIGHT(right),
    PLAY_AREA_BOTTOM(bottom) { }
};

const float EPSILON = 0.001f;

enum class shapeType { CIRCLE, CONVEX, RECTANGLE};

enum class state { MOVING, NOT_MOVING, STOPPED, BEING_MOVED };

int getRandomInteger(int min, int max)
{
    return (rand() % (max - min)) + min;
}

float getRandomFloat(float min, float max)
{
    return rand() / (RAND_MAX / (max - min)) + min;
}

double getRandomDouble(double min, double max)
{
    return rand() / (RAND_MAX / (max - min)) + min;
}
