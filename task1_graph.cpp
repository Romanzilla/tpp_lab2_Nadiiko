#define UNICODE
#define _UNICODE
#include <windows.h>
#include <cmath>
#include <cstdio>
#include <cwchar>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"GraphVariant18Class";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"Не вдалося зареєструвати клас вікна!", L"Помилка", MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Лабораторна робота №2 | Завдання 1 | Варіант 18: y = x / (x + 1)^2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 920, 620,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd)
    {
        MessageBox(nullptr, L"Не вдалося створити вікно!", L"Помилка", MB_OK | MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static int cx = 900, cy = 600;
    const int N = 200;
    static double xVals[N];
    static double yVals[N];

    switch (msg)
    {
    case WM_CREATE:
    {
        double xMin = 0.0;
        double xMax = 4.0;
        double step = (xMax - xMin) / (N - 1);
        for (int i = 0; i < N; ++i)
        {
            double x = xMin + i * step;
            xVals[i] = x;
            yVals[i] = x / ((x + 1.0) * (x + 1.0));
        }
        return 0;
    }

    case WM_SIZE:
    {
        cx = LOWORD(lParam);
        cy = HIWORD(lParam);
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hwnd);
        }
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (cx < 200 || cy < 200)
        {
            EndPaint(hwnd, &ps);
            return 0;
        }

        // Відступи області побудови графіка
        int leftMargin = 85;
        int rightMargin = cx - 60;
        int topMargin = 65;
        int bottomMargin = cy - 75;

        int plotWidth = rightMargin - leftMargin;
        int plotHeight = bottomMargin - topMargin;

        double xMin = 0.0, xMax = 4.0;
        double yMin = 0.0, yMax = 0.30; // Аналітичний максимум у точці x = 1: y(1) = 0.25

        // Шрифт для підписів осей і поділок
        HFONT hFont = CreateFont(
            16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI"
        );
        HFONT hTitleFont = CreateFont(
            20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI"
        );
        HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
        SetBkMode(hdc, TRANSPARENT);

        // Заголовок графіка
        SetTextColor(hdc, RGB(20, 40, 80));
        const wchar_t* title = L"Графік функції y = x / (x + 1)^2 на проміжку [0; 4]";
        TextOut(hdc, leftMargin, 18, title, static_cast<int>(wcslen(title)));

        SelectObject(hdc, hFont);

        // Допоміжна сітка (світло-сіра пунктирна)
        HPEN hGridPen = CreatePen(PS_DOT, 1, RGB(215, 220, 228));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hGridPen);

        // Вертикальні лінії сітки та розмітка осі X (крок 0.5)
        int xSteps = 8; // від 0.0 до 4.0 з кроком 0.5
        for (int i = 0; i <= xSteps; ++i)
        {
            double xVal = xMin + i * 0.5;
            int px = leftMargin + static_cast<int>((xVal - xMin) / (xMax - xMin) * plotWidth);

            if (i > 0)
            {
                MoveToEx(hdc, px, topMargin, nullptr);
                LineTo(hdc, px, bottomMargin);
            }
        }

        // Горизонтальні лінії сітки та розмітка осі Y (крок 0.05)
        int ySteps = 6; // від 0.00 до 0.30 з кроком 0.05
        for (int j = 0; j <= ySteps; ++j)
        {
            double yVal = yMin + j * 0.05;
            int py = bottomMargin - static_cast<int>((yVal - yMin) / (yMax - yMin) * plotHeight);

            if (j > 0)
            {
                MoveToEx(hdc, leftMargin, py, nullptr);
                LineTo(hdc, rightMargin, py);
            }
        }

        // Основні осі координат (товстіші, темно-сірі)
        HPEN hAxisPen = CreatePen(PS_SOLID, 2, RGB(35, 35, 45));
        SelectObject(hdc, hAxisPen);

        // Вісь Y (ординат) зі стрілкою вгору
        MoveToEx(hdc, leftMargin, bottomMargin + 15, nullptr);
        LineTo(hdc, leftMargin, topMargin - 20);
        LineTo(hdc, leftMargin - 5, topMargin - 10);
        MoveToEx(hdc, leftMargin, topMargin - 20, nullptr);
        LineTo(hdc, leftMargin + 5, topMargin - 10);

        // Вісь X (абсцис) зі стрілкою праворуч
        MoveToEx(hdc, leftMargin - 15, bottomMargin, nullptr);
        LineTo(hdc, rightMargin + 25, bottomMargin);
        LineTo(hdc, rightMargin + 15, bottomMargin - 5);
        MoveToEx(hdc, rightMargin + 25, bottomMargin, nullptr);
        LineTo(hdc, rightMargin + 15, bottomMargin + 5);

        // Засічки та числові підписи на осі X
        SetTextColor(hdc, RGB(40, 40, 40));
        SetTextAlign(hdc, TA_CENTER | TA_TOP);
        for (int i = 0; i <= xSteps; ++i)
        {
            double xVal = xMin + i * 0.5;
            int px = leftMargin + static_cast<int>((xVal - xMin) / (xMax - xMin) * plotWidth);

            MoveToEx(hdc, px, bottomMargin - 5, nullptr);
            LineTo(hdc, px, bottomMargin + 5);

            wchar_t label[16];
            swprintf(label, 16, L"%.1f", xVal);
            TextOut(hdc, px, bottomMargin + 10, label, static_cast<int>(wcslen(label)));
        }

        // Засічки та числові підписи на осі Y
        SetTextAlign(hdc, TA_RIGHT | TA_BASELINE);
        for (int j = 0; j <= ySteps; ++j)
        {
            double yVal = yMin + j * 0.05;
            int py = bottomMargin - static_cast<int>((yVal - yMin) / (yMax - yMin) * plotHeight);

            MoveToEx(hdc, leftMargin - 5, py, nullptr);
            LineTo(hdc, leftMargin + 5, py);

            if (j > 0) // 0.0 вже підписано біля початку координат
            {
                wchar_t label[16];
                swprintf(label, 16, L"%.2f", yVal);
                TextOut(hdc, leftMargin - 10, py + 5, label, static_cast<int>(wcslen(label)));
            }
        }

        // Підписи назв осей X та Y
        SetTextAlign(hdc, TA_LEFT | TA_TOP);
        SetTextColor(hdc, RGB(15, 35, 75));
        TextOut(hdc, rightMargin + 15, bottomMargin + 12, L"X", 1);
        TextOut(hdc, leftMargin + 12, topMargin - 24, L"Y", 1);

        // Малювання кривої графіка функції синім кольором товщиною 3 px
        HPEN hCurvePen = CreatePen(PS_SOLID, 3, RGB(0, 102, 204));
        SelectObject(hdc, hCurvePen);

        for (int i = 0; i < N; ++i)
        {
            int px = leftMargin + static_cast<int>((xVals[i] - xMin) / (xMax - xMin) * plotWidth);
            int py = bottomMargin - static_cast<int>((yVals[i] - yMin) / (yMax - yMin) * plotHeight);

            if (i == 0)
                MoveToEx(hdc, px, py, nullptr);
            else
                LineTo(hdc, px, py);
        }

        // Позначення точки максимуму функції (x = 1, y = 0.25)
        int maxPx = leftMargin + static_cast<int>((1.0 - xMin) / (xMax - xMin) * plotWidth);
        int maxPy = bottomMargin - static_cast<int>((0.25 - yMin) / (yMax - yMin) * plotHeight);
        HBRUSH hPointBrush = CreateSolidBrush(RGB(220, 40, 40));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hPointBrush);
        Ellipse(hdc, maxPx - 5, maxPy - 5, maxPx + 5, maxPy + 5);

        SetTextColor(hdc, RGB(180, 25, 25));
        const wchar_t* maxLabel = L"Max (1.0; 0.25)";
        TextOut(hdc, maxPx + 10, maxPy - 18, maxLabel, static_cast<int>(wcslen(maxLabel)));

        // Відновлення та очищення ресурсів GDI
        SelectObject(hdc, hOldBrush);
        DeleteObject(hPointBrush);
        SelectObject(hdc, hOldPen);
        DeleteObject(hGridPen);
        DeleteObject(hAxisPen);
        DeleteObject(hCurvePen);
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
        DeleteObject(hTitleFont);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
