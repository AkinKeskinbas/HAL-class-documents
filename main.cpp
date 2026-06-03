/*==============================================================================
        ウィンドウ表示 [main.cpp]

                                            Author : Akin Keskinbas
                                            Date   : 2025/4/21
==============================================================================*/

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

bool Direct3D_Initialize(HWND window_handle);
void Direct3D_Finalize();
void Direct3D_Begin();
void Direct3D_Present();

#include <algorithm>

#include "configuration.h"

/*------------------------------------------------------------------------------
        ウィンドウ情報
------------------------------------------------------------------------------*/
static constexpr char WINDOW_CLASS[] = "GameWindow"; // メインウィンドウクラス名
static constexpr char TITLE[] = "ウィンドウ表示";     // タイトルバーのテキスト

/*------------------------------------------------------------------------------
        ウィンドウプロシージャ プロトタイプ宣言
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/*------------------------------------------------------------------------------
        メイン
------------------------------------------------------------------------------*/
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    /* ウィンドウクラスの登録 */
    WNDCLASSEX wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;

    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr; // メニューは作らない
    wcex.lpszClassName = WINDOW_CLASS;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    RegisterClassEx(&wcex);

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);


    // プライマリモニターの画面解像度取得
    const int DESKTOP_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const int DESKTOP_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
   

    RECT window_rect{
     .left = 0,
     .top = 0,
     .right = SCREEN_WIDTH,
     .bottom = SCREEN_HEIGHT
    };

    constexpr DWORD WINDOW_STYLE =
        WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

    AdjustWindowRect(&window_rect, WINDOW_STYLE, FALSE);

    const int WINDOW_WIDTH = window_rect.right - window_rect.left;
    const int WINDOW_HEIGHT = window_rect.bottom - window_rect.top;

    //ekranin yerini belirliyor ortada gosterir burda
    const int WINDOW_X = std::max((DESKTOP_WIDTH - WINDOW_WIDTH) / 2, 0);
    const int WINDOW_Y = std::max((DESKTOP_HEIGHT - WINDOW_HEIGHT) / 2, 0);

    HWND hWnd = CreateWindow(
        WINDOW_CLASS,
        TITLE,
        WINDOW_STYLE,
        WINDOW_X,
        WINDOW_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    Direct3D_Initialize(hWnd);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    /* メッセージループ */
    MSG msg;

    do
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // ウィンドウメッセージが来ていたら
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // ゲームの処理
            Direct3D_Begin();
            Direct3D_Present();
        }

    }
    while (msg.message != WM_QUIT);


    Direct3D_Finalize();

    return 0;
}

/*------------------------------------------------------------------------------
        ウィンドウプロシージャ
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            int result = MessageBox(
                hWnd,
                "終了しますか？",
                "確認",
                MB_YESNO | MB_ICONQUESTION
            );

            if (result == IDYES)
            {
                PostQuitMessage(0);
            }
            return 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}