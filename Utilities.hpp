

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

enum class shapeType { CIRCLE, CONVEX, RECTANGLE};

enum class state { MOVING, NOT_MOVING, STOPPED, BEING_MOVED };
