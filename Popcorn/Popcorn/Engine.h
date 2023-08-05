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
//-----------------------------------------------------------------------------------------------------------------------------------------
const int Timer_ID = WM_USER + 1;
//-----------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
    AsEngine();

    void Init_Engine(HWND hwnd);
    void Draw_Frame(HDC hdc, RECT &paint_area);
    int On_Key_Down(EKey_Type key_type);
    int On_Timer();

    static const int Level_Widtht = 12;    // Ширина уровня в ячейках (кирпичах)
    static const int Level_Height = 14;    // Высота уровня в ячейках (кирпичах)

private:
    void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
    void Redraw_Platform();
    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HPEN& back_pen, HBRUSH& front_brush, HBRUSH& back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
    void Draw_Level(HDC hdc);
    void Draw_Platform(HDC hdc, int x, int y);
    void Draw_ball(HDC hdc);
    void Draw_Border(HDC hdc, int x, int y, bool top_border);
    void Draw_Bounds(HDC hdc, RECT& paint_area);
    void Check_Level_Brick_Hit(int& next_y_pos);
    void Move_Ball();

    //Хендл окна
    HWND Hwnd;

    //Для создания кисти и ручки
    HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen, Letter_Pen, BG_Pen, Ball_Pen, Border_Blue_Pen, Border_White_Pen;
    HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush, BG_Brush, Ball_Brush, Border_Blue_Brush, Border_White_Brush;

    // Переменные и константы
    static const int Global_scale = 3;
    static const int Brick_Width = 15;
    static const int Brick_Height = 7;
    static const int Cell_Width = 16;
    static const int Cell_Height = 8;
    static const int Level_X_Offest = 8;
    static const int Level_Y_Offest = 6;
    static const int Platform_Y_Pos = 185;
    static const int Circle_Size = 7;
    static const int Platform_Height = 7;
    static const int Ball_Size = 4;
    static const int Max_X_Pos = Level_X_Offest + (Cell_Width * Level_Widtht);
    static const int Max_Y_Pos = 199 - Ball_Size;
    static const int Border_X_Offset = 6;
    static const int Border_Y_Offset = 4;

    int Inner_Width;
    int Platform_X_Pos;
    int Platform_X_Step;
    int Platform_Width;

    int Ball_X_Pos;
    int Ball_Y_Pos;
    double Ball_Speed;
    double Ball_Direction;


    RECT Platform_Rect, Prev_Platform_Rect;     //Нынешняя и старая позиция платформы
    RECT Level_Rect;
    RECT Ball_Rect, Prev_Ball_Rect;

};
//-----------------------------------------------------------------------------------------------------------------------------------------
