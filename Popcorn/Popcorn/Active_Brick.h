#pragma once
#include "Config.h"

//-----------------------------------------------------------------------------------------------------------------------------------------
enum EBrick_Type //создание коллекции типов кирпичей
{
    EBT_None,  //0
    EBT_Red,   //1
    EBT_Blue   //2
};

class AActive_Brick
{
public:
    AActive_Brick(EBrick_Type brick_type);

    void Act(HWND hwnd);
    void Draw(HDC hdc, RECT &paint_area);

    static void Setup_Colors();
private:
    EBrick_Type Brick_Type;
    int Fade_Step;
    RECT Brick_Rect;

    static unsigned char Get_Fadding_Channel(const unsigned char color, unsigned char bg_color, int step);
    static void Get_Fadding_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush);

    static const int Max_Fade_Step = 80;

    static HPEN Fading_Red_Brick_Pens[Max_Fade_Step];
    static HBRUSH Fading_Red_Brick_Brushes[Max_Fade_Step];
    static HPEN Fading_Blue_Brick_Pens[Max_Fade_Step];
    static HBRUSH Fading_Blue_Brick_Brushes[Max_Fade_Step];
};
