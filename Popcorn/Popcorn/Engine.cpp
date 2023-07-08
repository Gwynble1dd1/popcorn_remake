﻿#include "Engine.h"
//-----------------------------------------------------------------------------------------------------------------------
enum EBrick_Type //создание коллекции начиная с нуля
{
    EBT_None,  //0
    EBT_Red,   //1
    RBT_Blue   //2
};
//Для создания кисти и ручки
HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush;


// Мои глобальные переменные
const int Global_scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offest = 8;
const int Level_Y_Offest = 6;
const int Circle_Size = 7;

int Inner_Width = 21;

//Объявление массива уровня
char Level_01[14][12] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // пустой ряд
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // фиолетовые кирпичи
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // красные кирпичи
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//-----------------------------------------------------------------------------------------------------------------------
// Выставление цвета ручки и кисти
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));

}

//-----------------------------------------------------------------------------------------------------------------------
//Настройка игры т.е. инициализация
void Init()
{
    Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
    Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
    Create_Pen_Brush(150, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);

}
//-----------------------------------------------------------------------------------------------------------------------
//Рисуем кирпичики
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
    HBRUSH brush;
    HPEN pen;


    switch (brick_type)
    {
    case EBT_None:
        return;
    case EBT_Red:
        pen = Brick_Red_Pen;
        brush = Brick_Red_Brush;
        break;
    case RBT_Blue:
        pen = Brick_Blue_Pen;
        brush = Brick_Blue_Brush;
        break;
    default:
        return;
    }

    SelectObject(hdc, brush);
    SelectObject(hdc, pen);
    RoundRect(hdc, x * Global_scale, y * Global_scale, (x + Brick_Width) * Global_scale, (y + Brick_Height) * Global_scale, 2 * Global_scale, 2 * Global_scale);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка всех кирпичей на уровне
void Draw_Level(HDC hdc)
{
    int i, j;

    for (i = 0; i < 14; i++)
        for (j = 0; j < 12; j++)
            Draw_Brick(hdc, Level_X_Offest + j * Cell_Width, Level_Y_Offest + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Рисуем платформу
void Draw_Platform(HDC hdc,int x,int y)
{


    // 1. Рисуем боковые шарики
    SelectObject(hdc, Platform_Circle_Brush);
    SelectObject(hdc, Platform_Circle_Pen);

    Ellipse(hdc, x * Global_scale, y * Global_scale, (x + Circle_Size) * Global_scale, (y + Circle_Size) * Global_scale);
    Ellipse(hdc, (x + Inner_Width) * Global_scale, y * Global_scale, (x + Circle_Size + Inner_Width) * Global_scale, (y + Circle_Size) * Global_scale);

    // 2. Рисуем блик на шарике при помощи Arc
    // TODO: Разобраться с его работой
    SelectObject(hdc, Highlight_Pen);
    Arc(hdc, (x + 1) * Global_scale, (y + 1) * Global_scale, (x + Circle_Size - 1) * Global_scale, (y + Circle_Size - 1) * Global_scale,
        (x + 1 + 1) * Global_scale, (y + 1) * Global_scale, (x + 1) * Global_scale, (y + 1 + 2) * Global_scale);

    // 3. Рисуем среднюю платформу 
    SelectObject(hdc, Platform_Inner_Brush);
    SelectObject(hdc, Platform_Inner_Pen);
    RoundRect(hdc, (x + 4) * Global_scale, (y + 1) * Global_scale, (x + 4 + Inner_Width - 1) * Global_scale, (y + 1 + 5) * Global_scale, 3 * Global_scale, 3 * Global_scale);

}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка экрана игры
void Draw_Frame(HDC hdc)
{


    Draw_Level(hdc);
    Draw_Platform(hdc, 50,100);
}
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
