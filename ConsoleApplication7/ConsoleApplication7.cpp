#define TIMER_ID 1
#define TIMER_DELAY 16
//#define DEBUG
#ifdef UNICODE
#ifdef DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:WINDOWS")
#endif // DEBUG
#else
#ifdef DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:WINDOWS")
#endif // DEBUG
#endif

#include <windows.h>

POINT startPoint = { 0 };
POINT endPoint = { 0 };
int isKeyPressed = 0;

RECT rect_user = { 5, 5, 10, 10 }; // 왼쪽 상단 좌표 (5, 5)에서 오른쪽 하단 좌표 (10, 10)까지의 사각형
RECT rect_target = { 50, 50, 150, 150 }; // 왼쪽 상단 좌표 (50, 50)에서 오른쪽 하단 좌표 (150, 150)까지의 사각형

RECT text_rect = { 10, 10, 100, 20 };
int isInside = 0;//사용 사각형이 목표 사각형 안에 있는지 여부를 나타내는 플래그

// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255));
    HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255));
    HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255));
    const wchar_t* text = L"Crash!!!";

    static BOOL isMoving = FALSE;
    static int moveDirectionX = 0;
    static int moveDirectionY = 0;

    switch (uMsg)
    {
    case WM_KEYDOWN:
        if (!isMoving)
        {
            isMoving = TRUE;
            SetTimer(hwnd, TIMER_ID, TIMER_DELAY, NULL);
        }

        if (wParam == VK_RIGHT)
        {
            moveDirectionY = 0;
            moveDirectionX = 1;
        }
        else if (wParam == VK_LEFT)
        {
            moveDirectionY = 0;
            moveDirectionX = -1;
        }
        else if (wParam == VK_UP)
        {
            moveDirectionY = -1;
            moveDirectionX = 0;
        }
        else if (wParam == VK_DOWN)
        {
            moveDirectionY = 1;
            moveDirectionX = 0;
        }
        break;

    case WM_KEYUP:
        KillTimer(hwnd, TIMER_ID);
        isMoving = FALSE;
        break;

    case WM_TIMER:
        if (isMoving)
        {
            // 파란 박스의 새로운 위치 계산하기
            int Right2 = rect_user.right + moveDirectionX;
            int Left2 = rect_user.left + moveDirectionX;
            int Bottom2 = rect_user.bottom + moveDirectionY;
            int Top2 = rect_user.top + moveDirectionY;

            // 새로운 위치가 윈도우 경계 안에 있는지 확인하기
            RECT windowRect;
            GetClientRect(hwnd, &windowRect);

            if (Left2 >= 0 && Right2 <= windowRect.right && Top2 >= 0 && Bottom2 <= windowRect.bottom)
            {
                // 경계 안에 있을 때만 파란 박스의 위치 업데이트 하기
                rect_user.left = Left2;
                rect_user.right = Right2;
                rect_user.bottom = Bottom2;
                rect_user.top = Top2;

                // 파란 박스가 목표 사각형 내에 있는지 확인하기
                isInside = rect_user.top >= rect_target.top && rect_user.bottom <= rect_target.bottom &&
                    rect_user.left >= rect_target.left && rect_user.right <= rect_target.right;

                // 윈도우 다시 그리기
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
            }
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &rect_target, hBrush_target);
        FillRect(hdc, &rect_user, hBrush_user);

        if (isInside) {
            TextOut(hdc, text_rect.top, text_rect.left, text, lstrlen(text));
        }
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    DeleteObject(hBrush_user);
    DeleteObject(hBrush_target);
    DeleteObject(hBrush_eraser);
    ReleaseDC(hwnd, hdc);

    return S_OK;
}
#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
    /* 윈도우 클래스 선언.*/
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));   // 모두 0으로 초기화.

    // 윈도우 클래스 값 설정
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Computer Software");
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpfnWndProc = WindowProc;

    // 윈도우 클래스 등록.
    if (RegisterClass(&wc) == 0)
    {
        MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
        exit(-1);   //예외
    }

    // Window viewport 영역 조정
    RECT rect = { 150, 100, 800, 600 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // 윈도우 생성
    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("20227046_조서윤"),
        WS_OVERLAPPEDWINDOW,
        0, 0,
        width, height,
        NULL, NULL,
        hInstance,
        NULL
    );

    // 오류 검사.
    if (hwnd == NULL)
    {
        MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
        exit(-1);
    }

    // 창 보이기.
    ShowWindow(hwnd, SW_SHOW); // 창 띄우고
    UpdateWindow(hwnd); // 업데이트해야 보임. 한 쌍으로 쓴다고 보면 됨.



    // 메시지 처리 루프.
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    // 메시지 처리.
    while (msg.message != WM_QUIT)
    {
        if (GetMessage(&msg, NULL, 0, 0))
        {
            // 메시지 번역
            TranslateMessage(&msg);
            // 메시지를 처리
            DispatchMessage(&msg);
        }
        //if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        //{
        //	// 메시지 해석해줘.
        //	TranslateMessage(&msg);
        //	// 메시지를 처리해야할 곳에 전달해줘.
        //	DispatchMessage(&msg);
        //} // PM_REMOVE의 자리는 이 메세지를 쓰고 어떡할거냐.의 의미인데 지운다는 것임.
        //else{}
    }

    UnregisterClass(wc.lpszClassName, wc.hInstance);

    //종료 메시지 보내기
    return (int)msg.wParam;

}