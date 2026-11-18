#define UNICODE
#define _UNICODE
#include <windows.h>
#include <cwchar>

// ============================================================================
// Клас 1: Сонце (Sun) — з променями та градієнтним контуром
// ============================================================================
class Sun
{
private:
    int radius;
public:
    Sun(int r = 45) : radius(r) {}

    void show(HDC dc, int X, int Y) const
    {
        HPEN rayPen = CreatePen(PS_SOLID, 3, RGB(255, 200, 0));
        HPEN oldPen = (HPEN)SelectObject(dc, rayPen);

        // Промені сонця
        int rayLen = radius + 22;
        int diagLen = static_cast<int>(rayLen * 0.72);
        MoveToEx(dc, X - rayLen, Y, nullptr); LineTo(dc, X + rayLen, Y);
        MoveToEx(dc, X, Y - rayLen, nullptr); LineTo(dc, X, Y + rayLen);
        MoveToEx(dc, X - diagLen, Y - diagLen, nullptr); LineTo(dc, X + diagLen, Y + diagLen);
        MoveToEx(dc, X - diagLen, Y + diagLen, nullptr); LineTo(dc, X + diagLen, Y - diagLen);

        // Диск сонця
        HPEN sunPen = CreatePen(PS_SOLID, 2, RGB(255, 165, 0));
        SelectObject(dc, sunPen);
        HBRUSH sunBrush = CreateSolidBrush(RGB(255, 235, 40));
        HBRUSH oldBrush = (HBRUSH)SelectObject(dc, sunBrush);

        Ellipse(dc, X - radius, Y - radius, X + radius, Y + radius);

        SelectObject(dc, oldBrush);
        DeleteObject(sunBrush);
        SelectObject(dc, oldPen);
        DeleteObject(rayPen);
        DeleteObject(sunPen);
    }
};

// ============================================================================
// Клас 2: Хмара (Cloud) — складається з кількох перекритих еліпсів
// ============================================================================
class Cloud
{
private:
    double scale;
public:
    Cloud(double s = 1.0) : scale(s) {}

    void show(HDC dc, int X, int Y) const
    {
        int s = static_cast<int>(100 * scale);
        HPEN cloudPen = CreatePen(PS_SOLID, 1, RGB(215, 228, 242));
        HPEN oldPen = (HPEN)SelectObject(dc, cloudPen);
        HBRUSH cloudBrush = CreateSolidBrush(RGB(250, 252, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(dc, cloudBrush);

        Ellipse(dc, X, Y + s / 5, X + s, Y + s * 3 / 5);
        Ellipse(dc, X + s / 6, Y, X + s * 3 / 5, Y + s * 2 / 5);
        Ellipse(dc, X + s * 2 / 5, Y + s / 12, X + s * 5 / 6, Y + s / 2);
        Ellipse(dc, X + s * 3 / 5, Y + s / 5, X + s * 6 / 5, Y + s * 3 / 5);

        SelectObject(dc, oldBrush);
        DeleteObject(cloudBrush);
        SelectObject(dc, oldPen);
        DeleteObject(cloudPen);
    }
};

// ============================================================================
// Клас 3: Птах / Чайка (Bird) — крила у вигляді дуг/ламаних ліній
// ============================================================================
class Bird
{
private:
    int wingSpan;
public:
    Bird(int span = 26) : wingSpan(span) {}

    void show(HDC dc, int X, int Y) const
    {
        HPEN birdPen = CreatePen(PS_SOLID, 2, RGB(35, 45, 65));
        HPEN oldPen = (HPEN)SelectObject(dc, birdPen);

        POINT leftWing[3] = {
            { X - wingSpan, Y - wingSpan / 4 },
            { X - wingSpan / 2, Y - wingSpan / 2 },
            { X, Y }
        };
        POINT rightWing[3] = {
            { X, Y },
            { X + wingSpan / 2, Y - wingSpan / 2 },
            { X + wingSpan, Y - wingSpan / 4 }
        };

        Polyline(dc, leftWing, 3);
        Polyline(dc, rightWing, 3);

        SelectObject(dc, oldPen);
        DeleteObject(birdPen);
    }
};

// ============================================================================
// Клас 4: Корабель (Ship) — корпус, палубна надбудова, ілюмінатори, щогла та вітрила
// ============================================================================
class Ship
{
private:
    double scale;
    COLORREF hullColor;
    COLORREF sailColor;
public:
    Ship(double s = 1.0, COLORREF hull = RGB(120, 58, 28), COLORREF sail = RGB(248, 246, 238))
        : scale(s), hullColor(hull), sailColor(sail) {}

    void show(HDC dc, int X, int Y) const
    {
        auto sx = [this](int v) { return static_cast<int>(v * scale); };

        // 1. Щогла
        HPEN mastPen = CreatePen(PS_SOLID, sx(4) > 1 ? sx(4) : 2, RGB(70, 40, 20));
        HPEN oldPen = (HPEN)SelectObject(dc, mastPen);
        MoveToEx(dc, X + sx(105), Y, nullptr);
        LineTo(dc, X + sx(105), Y - sx(135));

        // 2. Прапорець на верхівці щогли
        HPEN flagPen = CreatePen(PS_SOLID, 1, RGB(180, 20, 30));
        SelectObject(dc, flagPen);
        HBRUSH flagBrush = CreateSolidBrush(RGB(220, 35, 45));
        HBRUSH oldBrush = (HBRUSH)SelectObject(dc, flagBrush);
        POINT flagPoly[3] = {
            { X + sx(105), Y - sx(135) },
            { X + sx(138), Y - sx(126) },
            { X + sx(105), Y - sx(117) }
        };
        Polygon(dc, flagPoly, 3);
        SelectObject(dc, oldBrush);
        DeleteObject(flagBrush);
        DeleteObject(flagPen);

        // 3. Головне вітрило (ліве) і стаксель (праве)
        HPEN sailPen = CreatePen(PS_SOLID, 1, RGB(160, 160, 170));
        SelectObject(dc, sailPen);
        HBRUSH sailBrush = CreateSolidBrush(sailColor);
        oldBrush = (HBRUSH)SelectObject(dc, sailBrush);

        POINT mainSail[3] = {
            { X + sx(101), Y - sx(115) },
            { X + sx(32),  Y - sx(16) },
            { X + sx(101), Y - sx(16) }
        };
        Polygon(dc, mainSail, 3);

        POINT frontSail[3] = {
            { X + sx(109), Y - sx(105) },
            { X + sx(175), Y - sx(16) },
            { X + sx(109), Y - sx(16) }
        };
        Polygon(dc, frontSail, 3);

        SelectObject(dc, oldBrush);
        DeleteObject(sailBrush);
        DeleteObject(sailPen);

        // 4. Палубна надбудова (рубка)
        HPEN cabinPen = CreatePen(PS_SOLID, 1, RGB(90, 90, 100));
        SelectObject(dc, cabinPen);
        HBRUSH cabinBrush = CreateSolidBrush(RGB(230, 235, 240));
        oldBrush = (HBRUSH)SelectObject(dc, cabinBrush);
        Rectangle(dc, X + sx(65), Y - sx(15), X + sx(140), Y);
        SelectObject(dc, oldBrush);
        DeleteObject(cabinBrush);
        DeleteObject(cabinPen);

        // 5. Корпус корабля (трапеція)
        HPEN hullPen = CreatePen(PS_SOLID, 2, RGB(60, 28, 12));
        SelectObject(dc, hullPen);
        HBRUSH hullBrush = CreateSolidBrush(hullColor);
        oldBrush = (HBRUSH)SelectObject(dc, hullBrush);

        POINT hull[4] = {
            { X,           Y },
            { X + sx(215), Y },
            { X + sx(180), Y + sx(42) },
            { X + sx(30),  Y + sx(42) }
        };
        Polygon(dc, hull, 4);

        SelectObject(dc, oldBrush);
        DeleteObject(hullBrush);
        DeleteObject(hullPen);

        // 6. Декоративна ватерлінія та ілюмінатори
        HPEN stripePen = CreatePen(PS_SOLID, 2, RGB(240, 200, 60));
        SelectObject(dc, stripePen);
        MoveToEx(dc, X + sx(14), Y + sx(18));
        LineTo(dc, X + sx(198), Y + sx(18));
        DeleteObject(stripePen);

        HPEN portPen = CreatePen(PS_SOLID, 1, RGB(40, 60, 90));
        SelectObject(dc, portPen);
        HBRUSH portBrush = CreateSolidBrush(RGB(135, 210, 250));
        oldBrush = (HBRUSH)SelectObject(dc, portBrush);

        int r = sx(6) > 3 ? sx(6) : 3;
        for (int i = 0; i < 4; ++i)
        {
            int px = X + sx(52 + i * 32);
            int py = Y + sx(10);
            Ellipse(dc, px - r, py - r, px + r, py + r);
        }

        SelectObject(dc, oldBrush);
        DeleteObject(portBrush);
        SelectObject(dc, oldPen);
        DeleteObject(mastPen);
        DeleteObject(portPen);
    }
};

// Глобальний стан для інтерактивності (зсув кораблів по миші / клавішах)
static int g_shipShiftX = 0;
static bool g_nightMode = false;

void DrawScene(HDC dc, int width, int height)
{
    int horizonY = static_cast<int>(height * 0.56);

    // 1. Небо
    COLORREF skyColor = g_nightMode ? RGB(22, 34, 68) : RGB(135, 206, 250);
    HBRUSH skyBrush = CreateSolidBrush(skyColor);
    RECT skyRect = { 0, 0, width, horizonY };
    FillRect(dc, &skyRect, skyBrush);
    DeleteObject(skyBrush);

    // 2. Море (вода + хвилі)
    COLORREF seaColor = g_nightMode ? RGB(14, 52, 96) : RGB(28, 125, 205);
    HBRUSH seaBrush = CreateSolidBrush(seaColor);
    RECT seaRect = { 0, horizonY, width, height };
    FillRect(dc, &seaRect, seaBrush);
    DeleteObject(seaBrush);

    // Хвилі на морі
    HPEN wavePen = CreatePen(PS_SOLID, 2, g_nightMode ? RGB(50, 105, 165) : RGB(110, 195, 245));
    HPEN oldPen = (HPEN)SelectObject(dc, wavePen);
    for (int wy = horizonY + 25; wy < height - 15; wy += 36)
    {
        for (int wx = (wy % 72 == 0 ? 30 : 75); wx < width - 60; wx += 140)
        {
            MoveToEx(dc, wx, wy, nullptr);
            LineTo(dc, wx + 28, wy - 5);
            LineTo(dc, wx + 56, wy);
        }
    }
    SelectObject(dc, oldPen);
    DeleteObject(wavePen);

    // 3. Сонце (1 об'єкт)
    Sun sun(44);
    sun.show(dc, width - 130, 95);

    // 4. Хмари — повторюються 3 рази (вимога >= 2 разів)
    Cloud cloudBig(1.35);
    Cloud cloudMid(1.0);
    Cloud cloudSmall(0.8);
    cloudBig.show(dc, 85, 55);
    cloudMid.show(dc, 340, 85);
    cloudSmall.show(dc, 570, 48);

    // 5. Птахи — повторюються 4 рази (вимога >= 2 разів)
    Bird b1(26), b2(22), b3(28), b4(20);
    b1.show(dc, 240, 165);
    b2.show(dc, 290, 145);
    b3.show(dc, 510, 185);
    b4.show(dc, 620, 150);

    // 6. Кораблі — повторюються 2 рази (вимога >= 2 разів)
    Ship mainShip(1.15, RGB(128, 60, 26), RGB(252, 250, 242));
    Ship distantShip(0.68, RGB(70, 85, 115), RGB(235, 242, 250));

    int x1 = (130 + g_shipShiftX) % (width > 250 ? width - 150 : 700);
    int x2 = (540 + g_shipShiftX / 2) % (width > 250 ? width - 120 : 700);

    distantShip.show(dc, x2, horizonY + 35);
    mainShip.show(dc, x1, horizonY + 115);

    // Підказка керування у лівому верхньому куті
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, g_nightMode ? RGB(230, 240, 255) : RGB(20, 45, 80));
    const wchar_t* hint = L"Керування: ЛКМ — рух кораблів | Пробіл — день/ніч | ESC — вихід";
    TextOut(dc, 16, 14, hint, static_cast<int>(wcslen(hint)));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        DrawScene(dc, rect.right, rect.bottom);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_LBUTTONDOWN:
    {
        g_shipShiftX += 45;
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hwnd);
        }
        else if (wParam == VK_SPACE)
        {
            g_nightMode = !g_nightMode;
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        else if (wParam == VK_RIGHT)
        {
            g_shipShiftX += 30;
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        else if (wParam == VK_LEFT)
        {
            g_shipShiftX = (g_shipShiftX >= 30) ? (g_shipShiftX - 30) : 0;
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        return 0;
    }

    case WM_SIZE:
    {
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"MarineSceneVariant18";

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
        return 0;

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Лабораторна робота №2 | Завдання 2 | Варіант 18 (Корабель, птах, хмари, сонце)",
        WS_OVERLAPPEDWINDOW,
        100, 100, 960, 620,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG message = {};
    while (GetMessage(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return static_cast<int>(message.wParam);
}
