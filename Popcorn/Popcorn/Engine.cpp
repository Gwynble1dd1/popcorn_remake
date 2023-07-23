#include "Engine.h"
#define _USE_MATH_DEFINES
#include "math.h"

//-----------------------------------------------------------------------------------------------------------------------
enum EBrick_Type //создание коллекции начиная с нуля
{
    EBT_None,  //0
    EBT_Red,   //1
    EBT_Blue   //2
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
void Draw_Brick_Letter(HDC hdc,int x, int y, EBrick_Type brick_type, int rotation_step)
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

        // Настраиваием матрицу переворота буквы
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

    //Draw_Level(hdc);
    //Draw_Platform(hdc, 50,100);
    int i;
    for (i = 0; i < 16; i++)
    {
        Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 100, EBT_Blue, i);
        Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 130, EBT_Red, i);
    }

}
//-----------------------------------------------------------------------------------------------------------------------