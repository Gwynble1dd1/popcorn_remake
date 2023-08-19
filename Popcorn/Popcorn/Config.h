#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include "math.h"

//-----------------------------------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush);
   
    static const int Global_scale = 3;
    static const int Border_X_Offset = 6;
    static const int Border_Y_Offset = 4;
    static const int Level_Widtht = 12;    // Ширина уровня в ячейках (кирпичах)
    static const int Level_Height = 14;    // Высота уровня в ячейках (кирпичах)
    static const int Level_X_Offest = 8;
    static const int Level_Y_Offest = 6;
    static const int Cell_Width = 16;
    static const int Cell_Height = 8;
    static const int Ball_Size = 4;
    static const int Max_X_Pos = Level_X_Offest + (Cell_Width * Level_Widtht);
    static const int Max_Y_Pos = 199 - Ball_Size;
    static const int Y_Pos = 185;


};
