//====================================================
// 入力処理 [input.cpp]
//====================================================
#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>

//====================================================
// マクロ定義
//====================================================
#define	NUM_KEY_MAX (256) // キーの最大数

//====================================================
// プロトタイプ宣言
//====================================================
static bool initialize(HINSTANCE hInstance);
static void finalize(void);

//====================================================
// グローバル変数
//====================================================
LPDIRECTINPUT8 g_pInput = NULL; // DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL; // 入力デバイス(キーボード)へのポインタ
BYTE g_aKeyState[NUM_KEY_MAX]; // キーボードの入力情報ワーク
BYTE g_aKeyStateTrigger[NUM_KEY_MAX]; // キーボードのトリガー情報ワーク
BYTE g_aKeyStateRelease[NUM_KEY_MAX]; // キーボードのリリース情報ワーク
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL; // 入力デバイス(マウス)へのポインタ
DIMOUSESTATE g_MouseState; // マウス状態
DIMOUSESTATE g_MouseState_bak; // マウス情報(変化検知用)

//====================================================
// 入力処理の初期化
//====================================================
bool initialize(HINSTANCE hInstance)
{
	if(g_pInput == NULL) {

		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) 
		{
			return false;
		}
	}
	return true;
}

//====================================================
// 入力処理の終了処理
//====================================================
void finalize(void)
{
	if(g_pInput != NULL) 
	{
		// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//====================================================
// キーボードの初期化
//====================================================
bool Keyboard_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if( !initialize(hInstance) ) 
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return false;
	}

	// デバイスの作成
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return false;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return false;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return false;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return true;
}

//====================================================
// キーボードの終了処理
//====================================================
void Keyboard_Finalize(void)
{
	if(g_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	finalize();
}

//====================================================
// キーボードの更新処理
//====================================================
void Keyboard_Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		// aKeyState[DIK_SPACE] & 0x80
		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キートリガー・リリース情報を生成
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// キープレス情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//====================================================
// キーボードのプレス状態を取得
//====================================================
bool Keyboard_IsPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//====================================================
// キーボードのトリガー状態を取得
//====================================================
bool Keyboard_IsTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//====================================================
// キーボードのリリ－ス状態を取得
//====================================================
bool Keyboard_IsRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//====================================================
// マウスの初期化
//====================================================
bool Mouse_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if (!initialize(hInstance))
	{
		MessageBox(hWnd, "DirectInputオブジェクトが作れねぇ！", "警告！", MB_ICONWARNING);
		return false;
	}

	// デバイスの作成
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_pDevMouse, NULL)))
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return false;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return false;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (g_pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)) {
		// デバイスの設定に失敗
		return false;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevMouse->Acquire();
	ShowCursor(false);
	return true;
}

//====================================================
// マウスの終了処理
//====================================================
void Mouse_Finalize(void)
{
	if (g_pDevMouse != NULL)
	{// 入力デバイス(マウス)の開放
		// マウスへのアクセス権を開放(入力制御終了)
		g_pDevMouse->Unacquire();

		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//====================================================
// マウスの更新処理
//====================================================
void Mouse_Update(void)
{	
	// デバイスからデータを取得
	if (FAILED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseState)))
	{
		g_pDevMouse->Acquire();
		g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseState);
	}
}

DIMOUSESTATE* GetMouseState(void)
{
	return &g_MouseState;
}