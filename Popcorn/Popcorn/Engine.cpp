#include "Engine.h"
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

//Хендл окна
HWND Hwnd;

//Для создания кисти и ручки
HPEN Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen, Highlight_Pen, Letter_Pen, BG_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush, BG_Brush;


// Мои глобальные переменные
const int Global_scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offest = 8;
const int Level_Y_Offest = 6;
const int Level_Widtht = 14;    // Ширина уровня в ячейках (кирпичах)
const int Level_Height = 12;    // Высота уровня в ячейках (кирпичах)
const int Platform_Y_Pos = 185;
const int Circle_Size = 7;
const int Platform_Height = 7;

int Inner_Width = 21;
int Platform_X_Pos = 0;
int Platform_X_Step = Global_scale * 2;
int Platform_Width = 28;

RECT Platform_Rect, Prev_Platform_Rect;     //Нынешняя и старая позиция платформы
RECT Level_Rect;

//Объявление массива уровня
char Level_01[Level_Widtht][Level_Height] =
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
//Перерисовка области с платформой для ее движения
void Redraw_Platform()
{
    Prev_Platform_Rect = Platform_Rect;


    Platform_Rect.left = (Platform_X_Pos + Level_X_Offest) * Global_scale;
    Platform_Rect.top = Platform_Y_Pos * Global_scale;
    Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_scale;
    Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_scale;

    InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------
//Настройка игры т.е. инициализация
void Init_Engine(HWND hwnd)
{
    Hwnd = hwnd;

    Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
    Letter_Pen = CreatePen(PS_SOLID, Global_scale, RGB(255, 255, 255));

    Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
    Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);
    Create_Pen_Brush(150, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);

    Level_Rect.left = Level_X_Offest * Global_scale;
    Level_Rect.top = Level_Y_Offest * Global_scale;
    Level_Rect.right = Level_Rect.left + Cell_Width * Level_Widtht * Global_scale;
    Level_Rect.bottom = Level_Rect.left + Cell_Height * Level_Height * Global_scale;

    Redraw_Platform();
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
    case EBT_Blue:
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
//Выбираем цвет кирпича
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
{
    if (is_switch_color)
    {
        front_pen = Brick_Red_Pen;
        front_brush = Brick_Red_Brush;

        back_pen = Brick_Blue_Pen;
        back_brush = Brick_Blue_Brush;
    }
    else
    {
        front_pen = Brick_Blue_Pen;
        front_brush = Brick_Blue_Brush;

        back_pen = Brick_Red_Pen;
        back_brush = Brick_Red_Brush;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка падающего кирпича
void Draw_Brick_Letter(HDC hdc,int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
    bool switch_color;
    double rotation_angle;
    double offset;
    XFORM xform, old_xform;
    int brick_half_height = Brick_Height * Global_scale / 2;
    int back_part_offset;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;


    if (!(brick_type == EBT_Blue || brick_type == EBT_Red))    
        return;  // Либо синие, либо красные

    // Корректрировка шага вращения и угла поворота
    rotation_step = rotation_step % 16;

    if (rotation_step < 8)
        rotation_angle = 2.0 * M_PI / 16 * (double)rotation_step;  // Преобразование шага в угол
    else
        rotation_angle = 2.0 * M_PI / 16 * (double)(8 - rotation_step);


    if (rotation_step > 4 && rotation_step <= 12)
    {
        if (brick_type == EBT_Blue)
            switch_color = false;
        else
            switch_color = true;
    }

    else
    {
        if (brick_type == EBT_Red)
            switch_color = true;
        else
            switch_color = false;
    }

    Set_Brick_Letter_Colors(switch_color, front_pen, back_pen, front_brush, back_brush);


    if (rotation_step == 4 || rotation_step == 12)
    {
        // Выводим фон
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, x, y + brick_half_height - Global_scale, x + Brick_Width * Global_scale, y + brick_half_height);
        // Выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_scale, y + brick_half_height + Global_scale - 1);

    }
    else
    {
        SetGraphicsMode(hdc, GM_ADVANCED);

        // Настраиваием матрицу переворота буквы для трансформации
        xform.eM11 = 1.0f;
        xform.eM12 = 0.0f;
        xform.eM21 = 0.0f;
        xform.eM22 = (float)cos(rotation_angle);
        xform.eDx = (float)x;
        xform.eDy = (float)y + (float)(brick_half_height);
        GetWorldTransform(hdc, &old_xform);
        SetWorldTransform(hdc, &xform);
        // Выводим фон
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * Global_scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_scale, brick_half_height - back_part_offset);

        // Выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            if (letter_type == ELT_O)
            {
                SelectObject(hdc, Letter_Pen);
                Ellipse(hdc, 0 + 5 * Global_scale, (-5 * Global_scale) / 2, 0 + 10 * Global_scale, (5 * Global_scale) / 2);
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
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
    SelectObject(hdc, BG_Brush);
    SelectObject(hdc, BG_Pen);

    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    // 1. Рисуем боковые шарики
    SelectObject(hdc, Platform_Circle_Brush);
    SelectObject(hdc, Platform_Circle_Pen);

    Ellipse(hdc, x * Global_scale, y * Global_scale, (x + Circle_Size) * Global_scale, (y + Circle_Size) * Global_scale);
    Ellipse(hdc, (x + Inner_Width) * Global_scale, y * Global_scale, (x + Circle_Size + Inner_Width) * Global_scale, (y + Circle_Size) * Global_scale);

    // 2. Рисуем блик на шарике при помощи Arc
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
void Draw_Frame(HDC hdc, RECT &paint_area)
{
    RECT intersection_rect;


    if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect) )
        Draw_Level(hdc);

    if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
        Draw_Platform(hdc, Platform_X_Pos + Level_X_Offest, Platform_Y_Pos);
    
   /* int i;
    for (i = 0; i < 16; i++)
    {
        Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 260, EBT_Blue, ELT_O, i);
        Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 290, EBT_Red, ELT_None, i);
    }*/

}

//-----------------------------------------------------------------------------------------------------------------------
//Реакция на нажатие кнопки
int On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform_X_Pos -= Platform_X_Step;
        Redraw_Platform();
        break;

    case EKT_Right:
        Platform_X_Pos += Platform_X_Step;
        Redraw_Platform();
        break;

    case EKT_Space:
        break;

    }

    return 0;
}
//-----------------------------------------------------------------------------------------------------------------------