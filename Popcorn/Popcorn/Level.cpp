#include "Level.h"

//Объявление массива уровня
char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Widtht] =
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







//-----------------------------------------------------------------------------------------------------------------------
// ALevel
ALevel::ALevel()
: Brick_Red_Pen(0), Brick_Blue_Pen(0), Letter_Pen(0), Brick_Red_Brush(0), Brick_Blue_Brush(0), Level_Rect{}
{
}
// Иницилизация уровня 
void ALevel::Init()
{
    Letter_Pen = CreatePen(PS_SOLID, AsConfig::Global_scale, RGB(255, 255, 255));

    AsConfig::Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush); // Red_Brick
    AsConfig::Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);  // Blue_Brick

    Level_Rect.left = AsConfig::Level_X_Offest * AsConfig::Global_scale;
    Level_Rect.top = AsConfig::Level_Y_Offest * AsConfig::Global_scale;
    Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Widtht * AsConfig::Global_scale;
    Level_Rect.bottom = Level_Rect.left + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_scale;

}

// Отражение при попадании
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction)
{

    // Корректируем отражение от кирпичей
    int i, j;
    int brick_y_pos = AsConfig::Level_Y_Offest + AsConfig::Level_Height * AsConfig::Cell_Height;

    for (i = AsConfig::Level_Height - 1; i >= 0; i--)
    {
        for (j = 0; j < AsConfig::Level_Widtht; j++)
        {
            if (Level_01[i][j] == 0)
                continue;

            if (next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                ball_direction = -ball_direction;
            }
        }
        brick_y_pos -= AsConfig::Cell_Height;
    }

}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка всех кирпичей на уровне
void ALevel::Draw(HWND hwnd, HDC hdc, RECT &paint_area)
{
    int i, j;
    RECT intersection_rect;


    if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
        return;

    for (i = 0; i < AsConfig::Level_Height; i++)
        for (j = 0; j < AsConfig::Level_Widtht; j++)
            Draw_Brick(hdc, AsConfig::Level_X_Offest + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offest + i * AsConfig::Cell_Height, (EBrick_Type)Level_01[i][j]);


    Active_Brick.Draw(hdc , paint_area);
}

//-----------------------------------------------------------------------------------------------------------------------
//Рисуем кирпичики
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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

    RoundRect(hdc, x * AsConfig::Global_scale, y * AsConfig::Global_scale, (x + AsConfig::AsConfig::Brick_Width) * AsConfig::Global_scale, (y + AsConfig::Brick_Height) * AsConfig::Global_scale, 2 * AsConfig::Global_scale, 2 * AsConfig::Global_scale);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Выбираем цвет кирпича
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HPEN &back_pen, HBRUSH &front_brush, HBRUSH &back_brush)
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
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
    bool switch_color;
    double rotation_angle;
    double offset;
    XFORM xform, old_xform;
    int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_scale / 2;
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

        Rectangle(hdc, x, y + brick_half_height - AsConfig::Global_scale, x + AsConfig::Brick_Width * AsConfig::Global_scale, y + brick_half_height);
        // Выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_half_height, x + AsConfig::Brick_Width * AsConfig::Global_scale, y + brick_half_height + AsConfig::Global_scale - 1);

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

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * AsConfig::Global_scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_scale, brick_half_height - back_part_offset);

        // Выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, -brick_half_height, AsConfig::Brick_Width * AsConfig::Global_scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            if (letter_type == ELT_O)
            {
                SelectObject(hdc, Letter_Pen);
                Ellipse(hdc, 0 + 5 * AsConfig::Global_scale, (-5 * AsConfig::Global_scale) / 2, 0 + 10 * AsConfig::Global_scale, (5 * AsConfig::Global_scale) / 2);
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}
