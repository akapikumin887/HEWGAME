#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#include <windows.h>
#include <stdlib.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "time.h"
#include "system_timer.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "game.h"
#include "aiming.h"
#include "cube.h"
#include "plane.h"
#include "wall.h"
#include "target.h"

//====================================================
// 定数定義
//====================================================
#define CLASS_NAME     "GameWindow"       // ウインドウクラスの名前
#define WINDOW_CAPTION "BASEDX" // ウィンドウの名前
#define FPS_MEASUREMENT_TIME (1.0f)       // FPS計測時間

//====================================================
// プロトタイプ宣言
//====================================================
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ゲームの初期化関数
// 戻り値:初期化に失敗したときfalse
static bool Initialize(void);
// ゲームの更新関数
static void Update(void);
// ゲームの描画関数
static void Draw(void);
// ゲームの終了処理
static void Finalize(void);

//====================================================
// グローバル変数宣言
//====================================================
static HWND g_hWnd; // ウィンドウハンドル
static LPDIRECT3D9 g_pD3D = NULL; // Direct3Dインターフェース
// Direct3Dデバイス
static LPDIRECT3DTEXTURE9 g_pTexture = NULL; // テクスチャインターフェース

static float g_UVScrollValue = 0.0f;

static Time *g_System_Time;
static int g_FrameCount = 0; // フレームカウンター
static double g_StaticFrameTime = 0.0f; // フレーム固定用計測時間
SCENE g_Scene = SCENE_NONE;

//====================================================
//メイン
//====================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 使用しない一時変数を明示
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ウィンドウクラス構造体の設定
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc; // ウィンドウプロシージャの指定
	wc.lpszClassName = CLASS_NAME; // クラス名の設定
	wc.hInstance = hInstance; // インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // マウスカーソルを指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定

	// クラス登録
	RegisterClass(&wc);

	// ウィンドウスタイル
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// 基本矩形座標
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// 指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// 新たなWindowの矩形座標から幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// ウィンドウの生成
	g_hWnd = CreateWindow
	(
		CLASS_NAME,     // ウィンドウクラス
		WINDOW_CAPTION, // ウィンドウテキスト
		window_style,   // ウィンドウスタイル
		window_x,       // ウィンドウ座標x
		window_y,       // ウィンドウ座標y
		window_width,   // ウィンドウの幅
		window_height,  // ウィンドウの高さ
		NULL,           // 親ウィンドウハンドル
		NULL,           // メニューハンドル
		hInstance,      // インスタンスハンドル
		NULL            // 追加のアプリケーションデータ
	);

	if (g_hWnd == NULL)
	{
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return -1;
	}

	// 指定のウィンドウハンドルのウィンドウを指定の方法で表示
	ShowWindow(g_hWnd, nCmdShow);

	// ゲームの初期化(Direct3Dの初期化)
	if (!Initialize())
	{
		// ゲームの初期化に失敗した
		return -1;
	}

	// Windowsゲーム用メインループ
	MSG msg = {}; // msg.message == WM_NULL

	Keyboard_Initialize(hInstance, g_hWnd);
	Mouse_Initialize(hInstance, g_hWnd);

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// 現在のシステム時間を取得
			double time = g_System_Time->SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / FPS_STANTARD) {
				// 1 / 60 秒経っていなかったら空回り
				Sleep(0);
			}
			else {
				// フレーム固定用の計測時間を更新する
				g_StaticFrameTime = time; // ゲーム処理を行った時間を記録
				// ゲームの更新
				Update();
				// ゲームの描画
				Draw();
			}
		}
	}

	Finalize();

	return (int)msg.wParam;
}

// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSEメッセージの送信
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd); // 指定のウィンドウにWM_DESTROYメッセージを送る
		}
		return 0; // DefWindowProc関数にメッセージを流さず終了することによって何もなかったことにする

	case WM_DESTROY: // ウィンドウの破棄メッセージ
		PostQuitMessage(0); // WM_QUITメッセージの送信
		return 0;
	};

	// 通常メッセージ処理はこの関数に任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ゲームの初期化関数
bool Initialize(void)
{
	//g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	srand((unsigned int)time(NULL));

	if (!Mydirect3D_Initialize(g_hWnd))
	{
		// Direct3Dインターフェースの取得に失敗
		// MessageBox(g_hWnd, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	Sprite_Initialize_2D();

	DebugFont_Initialize();

	// テクスチャの読み込み
	if (Texture_Load() > 0) {
		MessageBox(g_hWnd, "いくつか読み込めなかったテクスチャファイルがあります", "エラー", MB_OK);
	}

	SetScene(SCENE_GAME);

	InitSound(g_hWnd);

	Camera_Initialize();

	Light_Initialize();
	
	// システムタイマーの初期化
	
	SystemTimer_Initialize();
	g_System_Time = Get_Time();
	// システムタイマーの起動
	g_System_Time->SystemTimer_Start();

	// フレーム固定用計測時間
	g_StaticFrameTime = g_System_Time->SystemTimer_GetTime();

	return true;
}

// ゲームの更新関数
void Update(void)
{
	Keyboard_Update();
	Mouse_Update();
	Camera_Update();

	Light_Update();

	switch (g_Scene)
	{
	case SCENE_TITLE:
		
		break;
	case SCENE_GAME:
		Game_Update();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}
}

// ゲームの描画関数
void Draw(void)
{
	static LPDIRECT3DDEVICE9 pD3DDevice = MyDirect3D_GetDevice();

	// 画面のクリア
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(200, 200, 0, 255), 1.0f, 0);

	// 描画バッチ命令の開始
	pD3DDevice->BeginScene();

	Camera_Draw();

	Light_Draw();
	DebugFont_Draw(640, 2, "Time: %.02lf", g_System_Time->SystemTimer_GetTime());
	// オブジェクトのDraw関数に入れる？
	//pD3DDevice->SetFVF(FVF_VERTEX_2D);

	switch (g_Scene)
	{
	case SCENE_TITLE:
		
		break;
	case SCENE_GAME:
		Game_Draw();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}
	// 描画バッチ命令の終了
	pD3DDevice->EndScene();

	// バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// ゲームの終了処理
void Finalize(void)
{
	// テクスチャインターフェースの解放
	if (g_pTexture) 
	{
		g_pTexture->Release();
		g_pTexture = NULL;
		Mydirect3D_Finalize();
	}

	UninitSound();

	Camera_Finalize();
	Light_Finalize();
	Sprite_Finalize_2D();
	Sprite_Finalize_3D();
	Texture_Release();
	// ゲームの終了処理(Direct3Dの終了処理)
	Keyboard_Finalize();
	Mouse_Finalize();
	DebugFont_Finalize();
	SystemTimer_Finalize();
	Mydirect3D_Finalize();
}

void SetScene(SCENE s)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		
		break;
	case SCENE_GAME:
		Game_Finalize();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}

	switch (s)
	{
	case SCENE_TITLE:
		
		break;
	case SCENE_GAME:
		Game_Initialize();
		break;
	case SCENE_RESULT:

		break;
	default:
		break;
	}
	g_Scene = s;
}