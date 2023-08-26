#include "Engine.h"


//-----------------------------------------------------------------------------------------------------------------------
// AsEngine
// Конструктор класса AsEngine
AsEngine::AsEngine()
:  Hwnd(0), BG_Pen(0), BG_Brush(0), Prev_Platform_Rect{}, Platform_Rect{}, Border{}
{
}

//-----------------------------------------------------------------------------------------------------------------------
//Настройка игры т.е. инициализация
void AsEngine::Init_Engine(HWND hwnd)
{
    Hwnd = hwnd;

    AsConfig::Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);

    Level.Init();
    Ball.Init();
    Platform.Init();
    Border.Init();

    Platform.Redraw(hwnd);

    SetTimer(hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка экрана игры
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{

    Level.Draw(Hwnd, hdc, paint_area);

    Platform.Draw(hdc, BG_Pen, BG_Brush, paint_area);

    /* int i;
     for (i = 0; i < 16; i++)
     {
         Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 260, EBT_Blue, ELT_O, i);
         Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 290, EBT_Red, ELT_None, i);
     }*/

    Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);
    Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);

}

//-----------------------------------------------------------------------------------------------------------------------
//Реакция на нажатие кнопки
int AsEngine::On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform.X_Pos -= Platform.X_Step;
        if (Platform.X_Pos <= AsConfig::Border_X_Offset)
            Platform.X_Pos = AsConfig::Border_X_Offset;
        Platform.Redraw(Hwnd);
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;
        if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
            Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

        Platform.Redraw(Hwnd);
        break;

    case EKT_Space:
        break;

    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
//Обработка события по таймеру
int AsEngine::On_Timer()
{
    Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);
    Level.Active_Brick.Act(Hwnd);

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------