/*==============================================================================
        ウィンドウ表示 [main.cpp]

                                            Author : Akin Keskinbas
                                            Date   : 2025/4/21
==============================================================================*/

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    /* メッセージループ */
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

/*------------------------------------------------------------------------------
        ウィンドウプロシージャ
------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY: // ウィンドウの破棄メッセージ
        PostQuitMessage(0); // WM_QUITメッセージの送信
        break;

    default:
        // 通常のメッセージ処理はこの関数に任せる
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}