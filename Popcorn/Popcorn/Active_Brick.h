#pragma once
#include "Config.h"

class AActive_Brick
{
public:
    AActive_Brick();

    void Act(HWND hwnd);
    void Draw(HDC hdc, RECT &paint_area);

    int Fade_Step;
    static const int Max_Fade_Step = 40;
    RECT Brick_Rect;


};
