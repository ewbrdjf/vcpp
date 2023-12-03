#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int margin = 8;
    int boxMargin = 12;
    int boxPadding = 12;

    RECT boxRect;
    GetClientRect(GetDlgItem(hwnd, 6), &boxRect);

    int centerX = (boxRect.left + boxRect.right) / 2;
    int centerY = (boxRect.top + boxRect.bottom) / 1.4;

    int largeWidth = 330; // 큰 하늘색 원의 가로 크기 (픽셀)
    int largeHeight = 330; // 큰 하늘색 원의 세로 크기 (픽셀)
    int smallWidth = 50;  // 검은색 원의 가로 크기 (픽셀)
    int smallHeight = 45; // 검은색 원의 세로 크기 (픽셀)
    int whiteWidth = 65;  // 흰색 원의 가로 크기 (픽셀)
    int whiteHeight = 55; // 흰색 원의 세로 크기 (픽셀)
    int verticalEllipseWidth = 50; // 세로로 눕힌 타원의 가로 크기 (픽셀)
    int verticalEllipseHeight = 80; // 세로로 눕힌 타원의 세로 크기 (픽셀)
    int eyeWidth = 15;    // 눈의 가로 크기 (픽셀)
    int eyeHeight = 25;   // 눈의 세로 크기 (픽셀)
    int wheyeWidth = 5;   // 눈동자 가로
    int wheyeHeight = 10;  // 눈동자 세로

    static HCURSOR hCrossCursor = LoadCursor(NULL, IDC_CROSS);
    static HCURSOR hArrowCursor = LoadCursor(NULL, IDC_ARROW);

    HDC hdc = NULL;

    switch (uMsg)
    {

    case WM_SETCURSOR:
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);

        RECT rc;
        rc.left = 17;
        rc.top = 67;
        rc.right = 800 - 37;
        rc.bottom = 480 - 65;

        if (pt.x > rc.left && pt.x < rc.right && pt.y > rc.top && pt.y < rc.bottom)
        {
            SetCursor(hCrossCursor);
            return TRUE;
        }
        else
        {
            SetCursor(hArrowCursor);
            return TRUE;
        }
    }
    break;


    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        int padding = 8;

        RECT marginRect = clientRect;
        InflateRect(&marginRect, -margin, -margin);

        RECT paddingRect = marginRect;
        InflateRect(&paddingRect, -padding, -padding);

        HBRUSH backgroundColor = CreateSolidBrush(RGB(255, 240, 200));
        HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

        SelectObject(hdc, backgroundColor);
        SelectObject(hdc, borderPen);

        Rectangle(hdc, marginRect.left, marginRect.top, marginRect.right, marginRect.bottom);
        Rectangle(hdc, paddingRect.left, paddingRect.top, paddingRect.right, paddingRect.bottom);

        DeleteObject(backgroundColor);
        DeleteObject(borderPen);

        EndPaint(hwnd, &ps);
    }
    break;
    case WM_CREATE:
    {

        int buttonIDs[] = { 1001, 1002, 1003, 1004, 1005 };
        LPCWSTR buttonLabels[] = { L"Box", L"Circle", L"Bonobono", L"Ryan", L"Cube" };

        for (int i = 0; i < 5; i++)
        {
            CreateWindow(
                L"BUTTON",
                buttonLabels[i],
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                16 + i * (130 + 16) + margin, 25, 120, 32,
                hwnd,
                (HMENU)(buttonIDs[i]),
                ((LPCREATESTRUCT)lParam)->hInstance,
                NULL
            );
        }


        CreateWindow(
            L"STATIC",
            L"",
            WS_CHILD | WS_VISIBLE | SS_NOTIFY,
            margin + boxMargin, 25 + 32 + boxMargin, 782 - 2 * margin - 2 * boxMargin, 435 - 25 - 32 - margin - 2 * boxMargin,
            hwnd,
            (HMENU)(6),
            ((LPCREATESTRUCT)lParam)->hInstance,
            NULL
        );
    }
    break;

    case WM_COMMAND:
    {
        int buttonID = LOWORD(wParam);

        HDC hdc;


        switch (buttonID)
        {
        case 1003:
        {
            hdc = GetDC(hwnd);

            // 큰 하늘색 원 그리기
            HBRUSH blueBrush = CreateSolidBrush(RGB(91, 155, 213));
            SelectObject(hdc, blueBrush);
            Ellipse(hdc, centerX - largeWidth / 2, centerY - largeHeight / 2, centerX + largeWidth / 2, centerY + largeHeight / 2);
            DeleteObject(blueBrush);


            // 세로로 눕힌 타원 그리기 (아래에)
            HBRUSH verticalEllipseBrush = CreateSolidBrush(RGB(251, 150, 251));
            SelectObject(hdc, verticalEllipseBrush);
            Ellipse(hdc, centerX - verticalEllipseWidth / 2, centerY - 3 + smallHeight / 2, centerX + verticalEllipseWidth / 2, centerY - 3 + smallHeight / 2 + verticalEllipseHeight);
            DeleteObject(verticalEllipseBrush);


            // 흰색 원 그리기 (왼쪽)
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, whiteBrush);
            Ellipse(hdc, centerX - whiteWidth, centerY - 3, centerX, centerY - 3 + whiteHeight);
            // 두 번째 흰색 원 그리기 (오른쪽에 이어붙임)
            Ellipse(hdc, centerX, centerY - 3, centerX + whiteWidth, centerY - 3 + whiteHeight);
            DeleteObject(whiteBrush);

            int xOffset = -1;
            int yOffset = 20;
            float angle = 45.0f;

            // '<' 만들기
            int leftLine1StartX = centerX - whiteWidth + xOffset;
            int leftLine1StartY = centerY + whiteHeight / 4 + yOffset;
            int leftLine1EndX = centerX + xOffset - whiteWidth / 2;
            int leftLine1EndY = centerY + yOffset;
            MoveToEx(hdc, leftLine1StartX, leftLine1StartY + 10, NULL);
            LineTo(hdc, leftLine1EndX, leftLine1EndY + 10);

            int leftLine2StartX = centerX - whiteWidth + xOffset;
            int leftLine2StartY = centerY - whiteHeight / 4 + yOffset;
            int leftLine2EndX = centerX + xOffset - whiteWidth / 2;
            int leftLine2EndY = centerY + yOffset;
            MoveToEx(hdc, leftLine2StartX, leftLine2StartY, NULL);
            LineTo(hdc, leftLine2EndX, leftLine2EndY);

            // '>' 만들기
            int rightLine1StartX = centerX + xOffset + whiteWidth / 2;
            int rightLine1StartY = centerY + yOffset;
            int rightLine1EndX = centerX + whiteWidth + xOffset;
            int rightLine1EndY = centerY + whiteHeight / 4 + yOffset;
            MoveToEx(hdc, rightLine1StartX, rightLine1StartY + 10, NULL);
            LineTo(hdc, rightLine1EndX, rightLine1EndY + 10);

            int rightLine2StartX = centerX + xOffset + whiteWidth / 2;
            int rightLine2StartY = centerY + yOffset;
            int rightLine2EndX = centerX + whiteWidth + xOffset;
            int rightLine2EndY = centerY - whiteHeight / 4 + yOffset;
            MoveToEx(hdc, rightLine2StartX, rightLine2StartY, NULL);
            LineTo(hdc, rightLine2EndX, rightLine2EndY);

            // 작은 검정색 원 그리기 (중앙에)
            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, blackBrush);
            Ellipse(hdc, centerX - smallWidth / 2, centerY - smallHeight / 2, centerX + smallWidth / 2, centerY + smallHeight / 2);


            // 왼쪽 눈 그리기 (왼쪽으로 이동)
            Ellipse(hdc, centerX - whiteWidth - eyeWidth - 50, centerY - 30 - eyeHeight / 2, centerX - whiteWidth - 50, centerY - 30 + eyeHeight / 2);
            DeleteObject(blackBrush);

            // 오른쪽 눈 그리기 (오른쪽으로 이동)
            Ellipse(hdc, centerX + whiteWidth + 50, centerY - 30 - eyeHeight / 2, centerX + whiteWidth + eyeWidth + 50, centerY - 30 + eyeHeight / 2);
            DeleteObject(blackBrush);


            // 왼쪽 눈 그리기 (왼쪽으로 이동)
            HBRUSH smallwhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, smallwhiteBrush);
            Ellipse(hdc, centerX - whiteWidth - wheyeWidth - 55, centerY - 30 - wheyeHeight / 2, centerX - whiteWidth - 55, centerY - 30 + wheyeHeight / 2);
            DeleteObject(smallwhiteBrush);

            // 오른쪽 눈 그리기 (오른쪽으로 이동)
            Ellipse(hdc, centerX + whiteWidth + 55, centerY - 30 - wheyeHeight / 2, centerX + whiteWidth + wheyeWidth + 55, centerY - 30 + wheyeHeight / 2);
            DeleteObject(smallwhiteBrush);



            ReleaseDC(hwnd, hdc);

            hdc = NULL;

            SetFocus(hwnd);
        }

        break;

        }
    }
    break;

    case WM_KEYDOWN:
    {
        if (wParam == VK_SPACE)
        {
            HDC hdc = GetDC(hwnd);

            // 큰 하늘색 원 그리기
            HBRUSH blueBrush = CreateSolidBrush(RGB(91, 155, 213));
            SelectObject(hdc, blueBrush);
            Ellipse(hdc, centerX - largeWidth / 2, centerY - largeHeight / 2, centerX + largeWidth / 2, centerY + largeHeight / 2);
            DeleteObject(blueBrush);


            // 세로로 눕힌 타원 그리기 (아래에)
            HBRUSH verticalEllipseBrush = CreateSolidBrush(RGB(251, 150, 251));
            SelectObject(hdc, verticalEllipseBrush);
            Ellipse(hdc, centerX - verticalEllipseWidth / 2, centerY - 3 + smallHeight / 2, centerX + verticalEllipseWidth / 2, centerY - 3 + smallHeight / 2 + verticalEllipseHeight);
            DeleteObject(verticalEllipseBrush);


            // 흰색 원 그리기 (왼쪽)
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, whiteBrush);
            Ellipse(hdc, centerX - whiteWidth, centerY - 3, centerX, centerY - 3 + whiteHeight);
            // 두 번째 흰색 원 그리기 (오른쪽에 이어붙임)
            Ellipse(hdc, centerX, centerY - 3, centerX + whiteWidth, centerY - 3 + whiteHeight);
            DeleteObject(whiteBrush);

            int xOffset = -1;
            int yOffset = 20;
            float angle = 45.0f;

            // '<' 만들기
            int leftLine1StartX = centerX - whiteWidth + xOffset;
            int leftLine1StartY = centerY + whiteHeight / 4 + yOffset;
            int leftLine1EndX = centerX + xOffset - whiteWidth / 2;
            int leftLine1EndY = centerY + yOffset;
            MoveToEx(hdc, leftLine1StartX, leftLine1StartY + 10, NULL);
            LineTo(hdc, leftLine1EndX, leftLine1EndY + 10);

            int leftLine2StartX = centerX - whiteWidth + xOffset;
            int leftLine2StartY = centerY - whiteHeight / 4 + yOffset;
            int leftLine2EndX = centerX + xOffset - whiteWidth / 2;
            int leftLine2EndY = centerY + yOffset;
            MoveToEx(hdc, leftLine2StartX, leftLine2StartY, NULL);
            LineTo(hdc, leftLine2EndX, leftLine2EndY);

            // '>' 만들기
            int rightLine1StartX = centerX + xOffset + whiteWidth / 2;
            int rightLine1StartY = centerY + yOffset;
            int rightLine1EndX = centerX + whiteWidth + xOffset;
            int rightLine1EndY = centerY + whiteHeight / 4 + yOffset;
            MoveToEx(hdc, rightLine1StartX, rightLine1StartY + 10, NULL);
            LineTo(hdc, rightLine1EndX, rightLine1EndY + 10);

            int rightLine2StartX = centerX + xOffset + whiteWidth / 2;
            int rightLine2StartY = centerY + yOffset;
            int rightLine2EndX = centerX + whiteWidth + xOffset;
            int rightLine2EndY = centerY - whiteHeight / 4 + yOffset;
            MoveToEx(hdc, rightLine2StartX, rightLine2StartY, NULL);
            LineTo(hdc, rightLine2EndX, rightLine2EndY);

            // 작은 검정색 원 그리기 (중앙에)
            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, blackBrush);
            Ellipse(hdc, centerX - smallWidth / 2, centerY - smallHeight / 2, centerX + smallWidth / 2, centerY + smallHeight / 2);

            // 왼쪽 눈동자 '>' 만들기
            MoveToEx(hdc, centerX - whiteWidth - eyeWidth - 50, centerY - 30 - eyeHeight / 2, NULL);
            LineTo(hdc, centerX - whiteWidth - 50, centerY - 30);

            MoveToEx(hdc, centerX - whiteWidth - eyeWidth - 50, centerY - 30 + eyeHeight / 2, NULL);
            LineTo(hdc, centerX - whiteWidth - 50, centerY - 30);

            // 오른쪽 눈동자 '<' 만들기
            MoveToEx(hdc, centerX + whiteWidth + 50, centerY - 30, NULL);
            LineTo(hdc, centerX + whiteWidth + eyeWidth + 50, centerY - 30 - eyeHeight / 2);

            MoveToEx(hdc, centerX + whiteWidth + 50, centerY - 30, NULL);
            LineTo(hdc, centerX + whiteWidth + eyeWidth + 50, centerY - 30 + eyeHeight / 2);

            ReleaseDC(hwnd, hdc);

            hdc = NULL;
        }
    }
    break;

    case WM_KEYUP:
    {
        if (wParam == VK_SPACE)
        {

            hdc = GetDC(hwnd);

            // 큰 하늘색 원 그리기
            HBRUSH blueBrush = CreateSolidBrush(RGB(91, 155, 213));
            SelectObject(hdc, blueBrush);
            Ellipse(hdc, centerX - largeWidth / 2, centerY - largeHeight / 2, centerX + largeWidth / 2, centerY + largeHeight / 2);
            DeleteObject(blueBrush);


            // 세로로 눕힌 타원 그리기 (아래에)
            HBRUSH verticalEllipseBrush = CreateSolidBrush(RGB(251, 150, 251));
            SelectObject(hdc, verticalEllipseBrush);
            Ellipse(hdc, centerX - verticalEllipseWidth / 2, centerY - 3 + smallHeight / 2, centerX + verticalEllipseWidth / 2, centerY - 3 + smallHeight / 2 + verticalEllipseHeight);
            DeleteObject(verticalEllipseBrush);


            // 흰색 원 그리기 (왼쪽)
            HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, whiteBrush);
            Ellipse(hdc, centerX - whiteWidth, centerY - 3, centerX, centerY - 3 + whiteHeight);
            // 두 번째 흰색 원 그리기 (오른쪽에 이어붙임)
            Ellipse(hdc, centerX, centerY - 3, centerX + whiteWidth, centerY - 3 + whiteHeight);
            DeleteObject(whiteBrush);

            int xOffset = -1; // 왼쪽으로 10 픽셀 이동
            int yOffset = 20;
            float angle = 45.0f;

            // '<' 만들기
            int leftLine1StartX = centerX - whiteWidth + xOffset;
            int leftLine1StartY = centerY + whiteHeight / 4 + yOffset;
            int leftLine1EndX = centerX + xOffset - whiteWidth / 2;
            int leftLine1EndY = centerY + yOffset;
            MoveToEx(hdc, leftLine1StartX, leftLine1StartY + 10, NULL);
            LineTo(hdc, leftLine1EndX, leftLine1EndY + 10);

            int leftLine2StartX = centerX - whiteWidth + xOffset;
            int leftLine2StartY = centerY - whiteHeight / 4 + yOffset;
            int leftLine2EndX = centerX + xOffset - whiteWidth / 2;
            int leftLine2EndY = centerY + yOffset;
            MoveToEx(hdc, leftLine2StartX, leftLine2StartY, NULL);
            LineTo(hdc, leftLine2EndX, leftLine2EndY);

            // '>' 만들기
            int rightLine1StartX = centerX + xOffset + whiteWidth / 2;
            int rightLine1StartY = centerY + yOffset;
            int rightLine1EndX = centerX + whiteWidth + xOffset;
            int rightLine1EndY = centerY + whiteHeight / 4 + yOffset;
            MoveToEx(hdc, rightLine1StartX, rightLine1StartY + 10, NULL);
            LineTo(hdc, rightLine1EndX, rightLine1EndY + 10);

            int rightLine2StartX = centerX + xOffset + whiteWidth / 2;
            int rightLine2StartY = centerY + yOffset;
            int rightLine2EndX = centerX + whiteWidth + xOffset;
            int rightLine2EndY = centerY - whiteHeight / 4 + yOffset;
            MoveToEx(hdc, rightLine2StartX, rightLine2StartY, NULL);
            LineTo(hdc, rightLine2EndX, rightLine2EndY);

            // 작은 검정색 원 그리기 (중앙에)
            HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc, blackBrush);
            Ellipse(hdc, centerX - smallWidth / 2, centerY - smallHeight / 2, centerX + smallWidth / 2, centerY + smallHeight / 2);


            // 왼쪽 눈 그리기 (왼쪽으로 이동)
            Ellipse(hdc, centerX - whiteWidth - eyeWidth - 50, centerY - 30 - eyeHeight / 2, centerX - whiteWidth - 50, centerY - 30 + eyeHeight / 2);
            DeleteObject(blackBrush);

            // 오른쪽 눈 그리기 (오른쪽으로 이동)
            Ellipse(hdc, centerX + whiteWidth + 50, centerY - 30 - eyeHeight / 2, centerX + whiteWidth + eyeWidth + 50, centerY - 30 + eyeHeight / 2);
            DeleteObject(blackBrush);


            // 왼쪽 눈 그리기 (왼쪽으로 이동)
            HBRUSH smallwhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, smallwhiteBrush);
            Ellipse(hdc, centerX - whiteWidth - wheyeWidth - 55, centerY - 30 - wheyeHeight / 2, centerX - whiteWidth - 55, centerY - 30 + wheyeHeight / 2);
            DeleteObject(smallwhiteBrush);

            // 오른쪽 눈 그리기 (오른쪽으로 이동)
            Ellipse(hdc, centerX + whiteWidth + 55, centerY - 30 - wheyeHeight / 2, centerX + whiteWidth + wheyeWidth + 55, centerY - 30 + wheyeHeight / 2);
            DeleteObject(smallwhiteBrush);



            ReleaseDC(hwnd, hdc);

            hdc = NULL;
        }
    }
    break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"SimpleWindowClass";

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"윈도우 클래스 등록 실패!", L"에러", MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowEx(
        0,
        L"SimpleWindowClass",
        L"202207046 조서윤",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 480,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    SetFocus(GetDlgItem(hwnd, 6));

    if (!hwnd)
    {
        MessageBox(NULL, L"윈도우 생성 실패!", L"에러", MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}