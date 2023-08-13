#include "Engine.h"
#define _USE_MATH_DEFINES
#include "math.h"

//Объявление массива уровня
char Level_01[ALevel::Level_Height][ALevel::Level_Widtht] =
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





//-----------------------------------------------------------------------------------------------------------------------------------------
// ABall
//Конструктор класса ABall
ABall::ABall()
: Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
}

// Иницилизация шарика 
void ABall::Init()
{
    AsEngine::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}


//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка игрового шарика
void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
        return;


    // 1. Очищаем фон
    SelectObject(hdc, engine->BG_Brush);
    SelectObject(hdc, engine->BG_Pen);

    Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

    // 2. Рисуем шарик
    SelectObject(hdc, Ball_Brush);
    SelectObject(hdc, Ball_Pen);

    Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}

//-----------------------------------------------------------------------------------------------------------------------
//Перемещение шарика
void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{
    int next_x_pos, next_y_pos;
    int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
    int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

    Prev_Ball_Rect = Ball_Rect;

    next_x_pos = Ball_X_Pos + int(Ball_Speed * cos(Ball_Direction));
    next_y_pos = Ball_Y_Pos - int(Ball_Speed * sin(Ball_Direction));

    // Корректируем позицию при отражении от рамки
    if (next_x_pos < AsBorder::Border_X_Offset)
    {
        next_x_pos = ALevel::Level_X_Offest - (next_x_pos - ALevel::Level_X_Offest);
        Ball_Direction = M_PI - Ball_Direction;
    }

    if (next_y_pos < AsBorder::Border_Y_Offset)
    {
        next_y_pos = AsBorder::Border_Y_Offset - (next_y_pos - ALevel::Level_Y_Offest);
        Ball_Direction = -Ball_Direction;
    }

    if (next_x_pos > max_x_pos)
    {
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
        Ball_Direction = M_PI - Ball_Direction;
    }

    if (next_y_pos > AsEngine::Max_Y_Pos)
    {
        next_y_pos = AsEngine::Max_Y_Pos - (next_y_pos - AsEngine::Max_Y_Pos);
        Ball_Direction = M_PI + (M_PI - Ball_Direction);
    }

    // Корректируем отражение от платформы

    if (next_y_pos > platform_y_pos)
    {
        if (next_x_pos >= platform->X_Pos && next_x_pos <= platform->X_Pos + platform->Width)
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
        Ball_Direction = M_PI + (M_PI - Ball_Direction);
    }

    // Корректируем отражение от кирпичей
    level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

    // Смещаем шарик
    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;


    Ball_Rect.left = Ball_X_Pos * AsEngine::Global_scale;
    Ball_Rect.top = Ball_Y_Pos * AsEngine::Global_scale;
    Ball_Rect.right = Ball_Rect.left + Ball_Size * AsEngine::Global_scale;
    Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsEngine::Global_scale;

    InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
    InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
}







//-----------------------------------------------------------------------------------------------------------------------
// ALevel

// Иницилизация уровня 
void ALevel::Init()
{
    Letter_Pen = CreatePen(PS_SOLID, AsEngine::Global_scale, RGB(255, 255, 255));

    AsEngine::Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    AsEngine::Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);

    Level_Rect.left = ALevel::Level_X_Offest * AsEngine::Global_scale;
    Level_Rect.top = ALevel::Level_Y_Offest * AsEngine::Global_scale;
    Level_Rect.right = Level_Rect.left + ALevel::Cell_Width * ALevel::Level_Widtht * AsEngine::Global_scale;
    Level_Rect.bottom = Level_Rect.left + ALevel::Cell_Height * ALevel::Level_Height * AsEngine::Global_scale;

}

// Отражение при попадании
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_direction)
{

    // Корректируем отражение от кирпичей
    int i, j;
    int brick_y_pos = Level_Y_Offest + Level_Height * Cell_Height;

    for (i = Level_Height - 1; i >= 0; i--)
    {
        for (j = 0; j < Level_Widtht; j++)
        {
            if (Level_01[i][j] == 0)
                continue;

            if (next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                ball_direction = -ball_direction;
            }
        }

        brick_y_pos -= Cell_Height;
    }

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
    RoundRect(hdc, x * AsEngine::Global_scale, y * AsEngine::Global_scale, (x + Brick_Width) * AsEngine::Global_scale, (y + Brick_Height) * AsEngine::Global_scale, 2 * AsEngine::Global_scale, 2 * AsEngine::Global_scale);
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
void ALevel::Draw_Brick_Letter(HDC hdc,int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
    bool switch_color;
    double rotation_angle;
    double offset;
    XFORM xform, old_xform;
    int brick_half_height = Brick_Height * AsEngine::Global_scale / 2;
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

        Rectangle(hdc, x, y + brick_half_height - AsEngine::Global_scale, x + Brick_Width * AsEngine::Global_scale, y + brick_half_height);
        // Выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * AsEngine::Global_scale, y + brick_half_height + AsEngine::Global_scale - 1);

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

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * AsEngine::Global_scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * AsEngine::Global_scale, brick_half_height - back_part_offset);

        // Выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, -brick_half_height, Brick_Width * AsEngine::Global_scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            if (letter_type == ELT_O)
            {
                SelectObject(hdc, Letter_Pen);
                Ellipse(hdc, 0 + 5 * AsEngine::Global_scale, (-5 * AsEngine::Global_scale) / 2, 0 + 10 * AsEngine::Global_scale, (5 * AsEngine::Global_scale) / 2);
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка всех кирпичей на уровне
void ALevel::Draw(HDC hdc, RECT &paint_area)
{
    int i, j;
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
        return;

    for (i = 0; i < Level_Height; i++)
        for (j = 0; j < Level_Widtht; j++)
            Draw_Brick(hdc, Level_X_Offest + j * Cell_Width, Level_Y_Offest + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}




// AsPlatform
//-----------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
: Inner_Width(21), X_Pos(AsBorder::Border_X_Offset), X_Step(AsEngine::Global_scale * 2), Width(28)
{
}
//-----------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
    Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

    AsEngine::Create_Pen_Brush(150, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    AsEngine::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}
//-----------------------------------------------------------------------------------------------------------------------
//Перерисовка области с платформой для ее движения
void AsPlatform::Redraw(AsEngine *engine)
{
    Prev_Platform_Rect = Platform_Rect;


    Platform_Rect.left = X_Pos * AsEngine::Global_scale;
    Platform_Rect.top = Y_Pos * AsEngine::Global_scale;
    Platform_Rect.right = Platform_Rect.left + Width * AsEngine::Global_scale;
    Platform_Rect.bottom = Platform_Rect.top + Height * AsEngine::Global_scale;

    InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Рисуем платформу
void AsPlatform::Draw(HDC hdc, AsEngine *engine, RECT &paint_area)
{

    int x = X_Pos;
    int y = Y_Pos;
    RECT intersection_rect;

    if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect) )
        return;

    SelectObject(hdc, engine->BG_Brush);
    SelectObject(hdc, engine->BG_Pen);

    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    // 1. Рисуем боковые шарики
    SelectObject(hdc, Platform_Circle_Brush);
    SelectObject(hdc, Platform_Circle_Pen);

    Ellipse(hdc, x * AsEngine::Global_scale, y * AsEngine::Global_scale, (x + Circle_Size) * AsEngine::Global_scale, (y + Circle_Size) * AsEngine::Global_scale);
    Ellipse(hdc, (x + Inner_Width) * AsEngine::Global_scale, y * AsEngine::Global_scale, (x + Circle_Size + Inner_Width) * AsEngine::Global_scale, (y + Circle_Size) * AsEngine::Global_scale);

    // 2. Рисуем блик на шарике при помощи Arc
    SelectObject(hdc, Highlight_Pen);
    Arc(hdc, (x + 1) * AsEngine::Global_scale, (y + 1) * AsEngine::Global_scale, (x + Circle_Size - 1) * AsEngine::Global_scale, (y + Circle_Size - 1) * AsEngine::Global_scale,
        (x + 1 + 1) * AsEngine::Global_scale, (y + 1) * AsEngine::Global_scale, (x + 1) * AsEngine::Global_scale, (y + 1 + 2) * AsEngine::Global_scale);


    // 3. Рисуем среднюю платформу 
    SelectObject(hdc, Platform_Inner_Brush);
    SelectObject(hdc, Platform_Inner_Pen);

    RoundRect(hdc, (x + 4) * AsEngine::Global_scale, (y + 1) * AsEngine::Global_scale, (x + 4 + Inner_Width - 1) * AsEngine::Global_scale, (y + 1 + 5) * AsEngine::Global_scale, 3 * AsEngine::Global_scale, 3 * AsEngine::Global_scale);

}





//-----------------------------------------------------------------------------------------------------------------------------------------
// AsBorder
// Отрисовка элемента рамки 
void AsBorder::Init()
{
    AsEngine::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
    AsEngine::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
}

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, AsEngine *engine)
{
    // Основная линия
    SelectObject(hdc, Border_Blue_Brush);
    SelectObject(hdc, Border_Blue_Pen);

    if (top_border)
        Rectangle(hdc, x * AsEngine::Global_scale, (y + 1) * AsEngine::Global_scale, (x + 4) * AsEngine::Global_scale, (y + 4) * AsEngine::Global_scale);
    else
        Rectangle(hdc, (x + 1) * AsEngine::Global_scale, y * AsEngine::Global_scale, (x + 4) * AsEngine::Global_scale, (y + 4) * AsEngine::Global_scale);

    // Леваая кайма
    SelectObject(hdc, Border_White_Brush);
    SelectObject(hdc, Border_White_Pen);

    if (top_border)
        Rectangle(hdc, x * AsEngine::Global_scale, y * AsEngine::Global_scale, (x + 4) * AsEngine::Global_scale, (y + 1) * AsEngine::Global_scale);
    else
        Rectangle(hdc, x * AsEngine::Global_scale, y * AsEngine::Global_scale, (x + 1) * AsEngine::Global_scale, (y + 4) * AsEngine::Global_scale);

    // Перфорация
    SelectObject(hdc, engine->BG_Brush);
    SelectObject(hdc, engine->BG_Pen);

    if (top_border)
        Rectangle(hdc, (x + 2) * AsEngine::Global_scale, (y + 2) * AsEngine::Global_scale, (x + 3) * AsEngine::Global_scale, (y + 3) * AsEngine::Global_scale);
    else
        Rectangle(hdc, (x + 2) * AsEngine::Global_scale, (y + 1) * AsEngine::Global_scale, (x + 3) * AsEngine::Global_scale, (y + 2) * AsEngine::Global_scale);

}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрсовка всех границ уровня
void AsBorder::Draw(HDC hdc, RECT& paint_area, AsEngine *engine)
{
    int i;

    // 1. Рисует рамку слева
    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 2, 1 + i * 4, false, engine);
    }

    // 2. Рисует рамку справа
    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 201, 1 + i * 4, false, engine);
    }

    for (i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 3 + i * 4, 0, true, engine);
    }

}





//-----------------------------------------------------------------------------------------------------------------------
// AsEngine
// Конструктор класса AsEngine
AsEngine::AsEngine()
{
}

//-----------------------------------------------------------------------------------------------------------------------
//Настройка игры т.е. инициализация
void AsEngine::Init_Engine(HWND hwnd)
{
    Hwnd = hwnd;


    Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);
    Create_Pen_Brush(255, 255, 255, Ball.Ball_Pen, Ball.Ball_Brush);


    Level.Init();
    Ball.Init();
    Platform.Init();
    Border.Init();

    Platform.Redraw(this);

    SetTimer(hwnd, Timer_ID, 50, 0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//Отрисовка экрана игры
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{

    Level.Draw(hdc, paint_area);

    Platform.Draw(hdc, this, paint_area);

    /* int i;
     for (i = 0; i < 16; i++)
     {
         Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 260, EBT_Blue, ELT_O, i);
         Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_scale, 290, EBT_Red, ELT_None, i);
     }*/

    Ball.Draw(hdc, paint_area, this);
    Border.Draw(hdc, paint_area, this);

}

//-----------------------------------------------------------------------------------------------------------------------
//Реакция на нажатие кнопки
int AsEngine::On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform.X_Pos -= Platform.X_Step;
        if (Platform.X_Pos <= AsBorder::Border_X_Offset)
            Platform.X_Pos = AsBorder::Border_X_Offset;
        Platform.Redraw(this);
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;
        if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 1)
            Platform.X_Pos = Max_X_Pos - Platform.Width + 1;

        Platform.Redraw(this);
        break;

    case EKT_Space:
        break;

    }

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
//Обработка события по таймеру
int AsEngine::On_Timer()
{
    Ball.Move(this, &Level, &Platform);

    return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
// Выставление цвета ручки и кисти
void AsEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));

}




//-----------------------------------------------------------------------------------------------------------------------