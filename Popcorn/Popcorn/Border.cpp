#include "Border.h"



//-----------------------------------------------------------------------------------------------------------------------------------------
// AsBorder
// Отрисовка элемента рамки 
void AsBorder::Init()
{
    AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
    AsConfig::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрсовка всех границ уровня
void AsBorder::Draw(HDC hdc, RECT &paint_area)
{
    int i;

    // 1. Рисует рамку слева
    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 2, 1 + i * 4, false);
    }

    // 2. Рисует рамку справа
    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 201, 1 + i * 4, false);
    }

    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 3 + i * 4, 0, true);
    }

}

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
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
    SelectObject(hdc, AsConfig::BG_Pen);
    SelectObject(hdc, AsConfig::BG_Brush);

    if (top_border)
        Rectangle(hdc, (x + 2) * AsConfig::Global_scale, (y + 2) * AsConfig::Global_scale, (x + 3) * AsConfig::Global_scale, (y + 3) * AsConfig::Global_scale);
    else
        Rectangle(hdc, (x + 2) * AsConfig::Global_scale, (y + 1) * AsConfig::Global_scale, (x + 3) * AsConfig::Global_scale, (y + 2) * AsConfig::Global_scale);

}

