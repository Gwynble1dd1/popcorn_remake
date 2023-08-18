#include "Border.h"

//-----------------------------------------------------------------------------------------------------------------------
// Выставление цвета ручки и кисти
void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}



//-----------------------------------------------------------------------------------------------------------------------------------------
// AsBorder
// Отрисовка элемента рамки 
void AsBorder::Init()
{
    AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
    AsConfig::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
}

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN bg_pen, HBRUSH bg_brush)
{
    // Основная линия
    SelectObject(hdc, Border_Blue_Brush);
    SelectObject(hdc, Border_Blue_Pen);

    if (top_border)
        Rectangle(hdc, x * AsConfig::Global_scale, (y + 1) * AsConfig::Global_scale, (x + 4) * AsConfig::Global_scale, (y + 4) * AsConfig::Global_scale);
    else
        Rectangle(hdc, (x + 1) * AsConfig::Global_scale, y * AsConfig::Global_scale, (x + 4) * AsConfig::Global_scale, (y + 4) * AsConfig::Global_scale);

    // Леваая кайма
    SelectObject(hdc, Border_White_Brush);
    SelectObject(hdc, Border_White_Pen);

    if (top_border)
        Rectangle(hdc, x * AsConfig::Global_scale, y * AsConfig::Global_scale, (x + 4) * AsConfig::Global_scale, (y + 1) * AsConfig::Global_scale);
    else
        Rectangle(hdc, x * AsConfig::Global_scale, y * AsConfig::Global_scale, (x + 1) * AsConfig::Global_scale, (y + 4) * AsConfig::Global_scale);

    // Перфорация
    SelectObject(hdc, bg_pen);
    SelectObject(hdc, bg_brush);

    if (top_border)
        Rectangle(hdc, (x + 2) * AsConfig::Global_scale, (y + 2) * AsConfig::Global_scale, (x + 3) * AsConfig::Global_scale, (y + 3) * AsConfig::Global_scale);
    else
        Rectangle(hdc, (x + 2) * AsConfig::Global_scale, (y + 1) * AsConfig::Global_scale, (x + 3) * AsConfig::Global_scale, (y + 2) * AsConfig::Global_scale);

}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрсовка всех границ уровня
void AsBorder::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
    int i;

    // 1. Рисует рамку слева
    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 2, 1 + i * 4, false, bg_pen, bg_brush);
    }

    // 2. Рисует рамку справа
    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 201, 1 + i * 4, false, bg_pen, bg_brush);
    }

    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 3 + i * 4, 0, true, bg_pen, bg_brush);
    }

}
