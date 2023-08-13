#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include "math.h"

//-----------------------------------------------------------------------------------------------------------------------
enum ELetter_Type //создание коллекции падающих букв 
{
    ELT_None,
    ELT_O
};

enum EBrick_Type //создание коллекции типов кирпичей
{
    EBT_None,  //0
    EBT_Red,   //1
    EBT_Blue   //2
};

enum EKey_Type //создание коллекции клавиш 
{
    EKT_Left,
    EKT_Right,
    EKT_Space
};

const int Timer_ID = WM_USER + 1;


//-----------------------------------------------------------------------------------------------------------------------------------------
class AsEngine;
class ALevel;
class ABall
{
public:
    ABall();

    void Init();
    void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);
    void Move(AsEngine *engine, ALevel *level);

    HPEN Ball_Pen;
    HBRUSH Ball_Brush;

    static const int Ball_Size = 4;
    double Ball_Direction;
private:
    int Ball_X_Pos, Ball_Y_Pos;
    double Ball_Speed;

    RECT Ball_Rect, Prev_Ball_Rect;
};


//-----------------------------------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
    void Init();
    void Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction);
    void Draw_Level(HDC hdc, RECT &paint_area);


    static const int Level_Widtht = 12;    // Ширина уровня в ячейках (кирпичах)
    static const int Level_Height = 14;    // Высота уровня в ячейках (кирпичах)
    static const int Level_X_Offest = 8;
    static const int Level_Y_Offest = 6;
    static const int Cell_Width = 16;
    static const int Cell_Height = 8;

private:
    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

    HPEN Brick_Red_Pen, Brick_Blue_Pen, Letter_Pen;
    HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

    RECT Level_Rect;


    static const int Brick_Width = 15;
    static const int Brick_Height = 7;
};
//-----------------------------------------------------------------------------------------------------------------------------------------

class AsPlatform
{
public:
    void Redraw_Platform(AsEngine *engine);
    void Draw_Platform(HDC hdc, int x, int y, AsEngine *engine);

    int Platform_X_Pos;
    int Platform_Width;

    RECT Platform_Rect, Prev_Platform_Rect;     //Нынешняя и старая позиция платформы

    static const int Platform_Height = 7;
    static const int Platform_Y_Pos = 185;

    HPEN Platform_Circle_Pen, Platform_Inner_Pen;
    HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

};

//-----------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
    AsEngine();

    void Init_Engine(HWND hwnd);
    void Draw_Frame(HDC hdc, RECT &paint_area);
    int On_Key_Down(EKey_Type key_type);
    int On_Timer();

    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);


    //Хендл окна
    HWND Hwnd;

    HPEN BG_Pen;
    HBRUSH BG_Brush;


    static const int Global_scale = 3;
    static const int Max_X_Pos = ALevel::Level_X_Offest + (ALevel::Cell_Width * ALevel::Level_Widtht);
    static const int Max_Y_Pos = 199 - ABall::Ball_Size;
    static const int Border_X_Offset = 6;
    static const int Border_Y_Offset = 4;

private:

    void Draw_Border(HDC hdc, int x, int y, bool top_border);
    void Draw_Bounds(HDC hdc, RECT &paint_area);

    //Для создания кисти и ручки
    HPEN  Highlight_Pen, Border_Blue_Pen, Border_White_Pen;
    HBRUSH Border_Blue_Brush, Border_White_Brush;
    // Переменные
    int Inner_Width;
    int Platform_X_Step;

    RECT Platform_Rect, Prev_Platform_Rect;     //Нынешняя и старая позиция платформы

    ABall Ball;
    ALevel Level;

    static const int Circle_Size = 7;

};


//-----------------------------------------------------------------------------------------------------------------------------------------
