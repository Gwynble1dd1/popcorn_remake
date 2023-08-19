﻿#pragma once

#include "Config.h"


//-----------------------------------------------------------------------------------------------------------------------------------------
enum EBrick_Type //создание коллекции типов кирпичей
{
    EBT_None,  //0
    EBT_Red,   //1
    EBT_Blue   //2
};

//-----------------------------------------------------------------------------------------------------------------------
enum ELetter_Type //создание коллекции падающих букв 
{
    ELT_None,
    ELT_O
};

//-----------------------------------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
    ALevel();
    void Init();
    void Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction);
    void Draw(HDC hdc, RECT &paint_area);

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