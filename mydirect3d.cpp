#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<dxerr.h>
#include<math.h>	
#include"common.h"

static LPDIRECT3D9 g_pD3D = NULL;
static LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;

LPDIRECT3DDEVICE9 Mydirect3D_GetDevice(void)
{
	return g_pD3DDevice;
}

void Mydirect3D_Finalize(void)
{
	// Direct3Dデバイスの解放
	if (g_pD3DDevice)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dインターフェースの解放
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

bool Mydirect3D_Initialize(HWND hwnd)
{
	// ゲームの初期化関数
	// Direct3Dインターフェースの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		// Direct3Dインターフェースの取得に失敗
		MessageBox(hwnd, "Direct3Dインターフェースの作成に失敗しました", "エラー", MB_OK);
		return false;
	}

	// デバイスの生成に利用する情報構造体の作成
	D3DPRESENT_PARAMETERS d3dpp = {};

	d3dpp.BackBufferWidth = SCREEN_WIDTH; // バックバッファの横幅				
	d3dpp.BackBufferHeight = SCREEN_HEIGHT; // バックバッファの縦幅
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // バックバッファのフォーマット指定
	d3dpp.BackBufferCount = 1; // バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // スワップエフェクト（スワップ方法）の設定		
	d3dpp.Windowed = TRUE; // ウィンドウモード or フルスクリーン
	d3dpp.EnableAutoDepthStencil = TRUE; // 深度バッファ・ステンシルバッファの使用				
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; // 深度バッファ・ステンシルバッファのフォーマット指定
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // フルスクリーン時のリフレッシュレートの指定
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; // リフレッシュレートとPresent処理の関係

	// Direct3Dデバイスの取得
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		// デバイスの作成に失敗
		MessageBox(hwnd, "Direct3Dデバイスの作成に失敗しました", "エラー", MB_OK);
		return false;
	}
	return true;
}	