#include "Platform.h"

// AsPlatform
//-----------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
    : Inner_Width(21), X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Global_scale * 2), Width(28), Platform_Circle_Pen(0),
    Platform_Inner_Pen(0), Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0)
{
}
//-----------------------------------------------------------------------------------------------------------------------          
void AsPlatform::Init()
{
    Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

    AsConfig::Create_Pen_Brush(150, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    AsConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
//-----------------------------------------------------------------------------------------------------------------------
//Перерисовка области с платформой для ее движения
void AsPlatform::Redraw(HWND hwnd)
{
    Prev_Platform_Rect = Platform_Rect;


    Platform_Rect.left = X_Pos * AsConfig::Global_scale;
    Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_scale;
    Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_scale;
    Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_scale;

    InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(hwnd, &Platform_Rect, FALSE);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Рисуем платформу
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{

    int x = X_Pos;
    int y = AsConfig::Platform_Y_Pos;
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
        return;

    SelectObject(hdc, AsConfig::BG_Brush);
    SelectObject(hdc, AsConfig::BG_Pen);

    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    // 1. Рисуем боковые шарики
    SelectObject(hdc, Platform_Circle_Brush);
    SelectObject(hdc, Platform_Circle_Pen);

    Ellipse(hdc, x * AsConfig::Global_scale, y * AsConfig::Global_scale, (x + Circle_Size) * AsConfig::Global_scale, (y + Circle_Size) * AsConfig::Global_scale);
    Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_scale, y * AsConfig::Global_scale, (x + Circle_Size + Inner_Width) * AsConfig::Global_scale, (y + Circle_Size) * AsConfig::Global_scale);

    // 2. Рисуем блик на шарике при помощи Arc
    SelectObject(hdc, Highlight_Pen);
    Arc(hdc, (x + 1) * AsConfig::Global_scale, (y + 1) * AsConfig::Global_scale, (x + Circle_Size - 1) * AsConfig::Global_scale, (y + Circle_Size - 1) * AsConfig::Global_scale,
        (x + 1 + 1) * AsConfig::Global_scale, (y + 1) * AsConfig::Global_scale, (x + 1) * AsConfig::Global_scale, (y + 1 + 2) * AsConfig::Global_scale);


    // 3. Рисуем среднюю платформу 
    SelectObject(hdc, Platform_Inner_Brush);
    SelectObject(hdc, Platform_Inner_Pen);

    RoundRect(hdc, (x + 4) * AsConfig::Global_scale, (y + 1) * AsConfig::Global_scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_scale, (y + 1 + 5) * AsConfig::Global_scale, 3 * AsConfig::Global_scale, 3 * AsConfig::Global_scale);

}

