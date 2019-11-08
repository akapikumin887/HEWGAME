#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#include <Windows.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "sprite.h"
#include <d3dx9.h>
#include"input.h"
#include"debug_font.h"
//#include"Grid.h"
#define CHARGE_SPAN 50
//Grid grid;

//===============================================
//ここを追加　3D用追加コード

//3Dポリゴン頂点フォーマット構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;  //頂点座標
	D3DXVECTOR3 nor;  //法線ベクトル
	D3DCOLOR diffuse; //反射光
	D3DXVECTOR2 tex;  //テクスチャ座標

} VERTEX_3D;

//3Dポリゴン頂点フォーマット( 頂点座標[３D] /法線  /     反射光    /　テクスチャ座標
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//プロジェクション用パラメータ
#define VIEW_ANGEL (D3DXToRadian(45.0f))  //ビュー平面の視野角
#define VIEW_ASPECT ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)  //ビュー平面のアスペクト比

#define VIEW_NEAR_Z (1.0f)    //ビュー平面のNearZ値
#define VIEW_FAR_Z (1000.0f)  //ビュー平面のFarZ値

//カメラ用パラメータ
D3DXVECTOR3 g_posCameraEye; //カメラの視点
D3DXVECTOR3 g_posCameraAt;  //カメラの注視点
D3DXVECTOR3 g_vecCameraUp;  //カメラの上方向

D3DXMATRIX g_mtxView;       //ビューマトリックス
D3DXMATRIX g_mtxprojection; //プロジェクションマトリックス

//サンプルポリゴンの表示自体に関するパラメータ

D3DXVECTOR3 g_pos;     //地面の位置(四角形の中央座標)
D3DXVECTOR3 g_rot;     //地面の向き(回転)
D3DXVECTOR3 g_scl;     //地面の大きさ(スケール)
D3DXMATRIX g_mtxworld; //ワールドマトリックス

LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuff;  //頂点バッファ

D3DXVECTOR3 g_pos2;     //地面の位置(四角形の中央座標)
D3DXVECTOR3 g_rot2;     //地面の向き(回転)
D3DXVECTOR3 g_scl2;     //地面の大きさ(スケール)
D3DXMATRIX g_mtxworld2; //ワールドマトリックス

D3DXVECTOR3 g_pos3;     //地面の位置(四角形の中央座標)
D3DXVECTOR3 g_rot3;     //地面の向き(回転)
D3DXVECTOR3 g_scl3;     //地面の大きさ(スケール)
D3DXMATRIX g_mtxworld3; //ワールドマトリックス

class Player
{
public:
	bool bUse;                 // 使用中フラグ
	bool isShot;               // 発射フラグ
	bool prepare;              // 構えフラグ
	D3DXVECTOR3 pos;           // 発射始点位置情報
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ
	int power;                 // 弓を引く力
	int	charge_span;           // チャージ間隔

	//Player();
	//~Player();
};
static Player player;



//ここまで3D用追加コード
//===============================================



/*------------------------------------------------------------------------------
   定数定義
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"       // ウインドウクラスの名前
#define WINDOW_CAPTION "ゲームウィンドウ" // ウィンドウの名前


/*------------------------------------------------------------------------------
   プロトタイプ宣言
------------------------------------------------------------------------------*/
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


/*------------------------------------------------------------------------------
   グローバル変数宣言
------------------------------------------------------------------------------*/
static HWND g_hWnd;             // ウィンドウハンドル
static float g_Rotation = 0.0f, g_Rotation2 = 0.0f, g_Rotation3= 0.0f; // スプライト回転角度 
static float g_posx=0, g_posy=4, g_posz=0;
static float g_velx = 0.1f, g_vely = 0, g_velz = 0;


/*------------------------------------------------------------------------------
   関数定義
------------------------------------------------------------------------------*/

//#######################################################################
// メイン関数
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 使用しない一時変数を明示
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // ウィンドウクラス構造体の設定
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
    wc.lpszClassName = CLASS_NAME;                     // クラス名の設定
    wc.hInstance = hInstance;                          // インスタンスハンドルの指定
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
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
    g_hWnd = CreateWindow(
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

    if( g_hWnd == NULL ) {
        // ウィンドウハンドルが何らかの理由で生成出来なかった
        return -1;
    }

    // 指定のウィンドウハンドルのウィンドウを指定の方法で表示
    ShowWindow(g_hWnd, nCmdShow);


	// ゲームの初期化(Direct3Dの初期化)
	if( !Initialize() ) {
        // ゲームの初期化に失敗した
		return -1;
	}

    // Windowsゲーム用メインループ
    MSG msg = {}; // msg.message == WM_NULL
    while( WM_QUIT != msg.message ) {

        if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) {
            // メッセージがある場合はメッセージ処理を優先
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // ゲームの更新
			Update();
			// ゲームの描画
			Draw();
        }
    }

	// ゲームの終了処理(Direct3Dの終了処理)
	Finalize();

    return (int)msg.wParam;
}

//#######################################################################
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch( uMsg ) {
        case WM_KEYDOWN:
            if( wParam == VK_ESCAPE ) {
                SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSEメッセージの送信
            }
            break;

        case WM_CLOSE:
            if( MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK ) {
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


//#######################################################################
// ゲームの初期化関数
bool Initialize(void)
{
    // Direct3Dラッパーモジュールの初期化
    if( !MyDirect3D_Initialize(g_hWnd) ) {
        return false;
    }

    // テクスチャの読み込み
    if( Texture_Load() > 0 ) {
        MessageBox(g_hWnd, "いくつか読み込めなかったテクスチャファイルがあります", "エラー", MB_OK);
    }


	//================================================================
	//ここを追加　3Dポリゴン用頂点の準備

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	//オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 36,  //頂点データ用に確保するバッファサイズ／(バイト単位)

		D3DUSAGE_WRITEONLY, //頂点バッファの使用法
		FVF_VERTEX_3D,      //使用する頂点フォーマット
		D3DPOOL_MANAGED,    //リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuff,     //頂点バッファインターフェースへのポインタ
		NULL);              //NULLに設定

	//頂点バッファの中身を埋める
	VERTEX_3D  *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	

	
		pVtx[0].vtx = D3DXVECTOR3(1.0f, 0.4f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f, 0.8f, 0.0f);

		pVtx[3].vtx = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
		pVtx[4].vtx = D3DXVECTOR3(0.0f, 0.3f, 0.0f);
		pVtx[5].vtx = D3DXVECTOR3(-1.0f, 0.5f, 0.0f);


		pVtx[6].vtx = D3DXVECTOR3(-1.0f, 0.5f, 0.0f);
		pVtx[7].vtx = D3DXVECTOR3(0.0f, 0.3f, 0.0f);
		pVtx[8].vtx = D3DXVECTOR3(-1.0f, 0.3f, 0.0f);
	
	

	//法線ベクトルの設定(現時点ではいじらない)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	pVtx[6].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	



	//反射光の設定(現時点ではいじらない)
	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[6].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, -1.0f);
	pVtx[2].tex = D3DXVECTOR2(-1.0f, -1.0f);

	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, -1.0f);
	pVtx[8].tex = D3DXVECTOR2(-1.0f, -1.0f);

		//頂点データをアンロックする
	g_pD3DVtxBuff->Unlock();

	//ここまで　3Dポリゴン用頂点の準備
	//================================================================


    return true;
}

//#######################################################################
// ゲームの更新関数
void Update(void)
{
	
	// 弓を引く（チャージ）
	if (GetAsyncKeyState('Z'))
	{	// 押している間チャージする
		if (player.charge_span < CHARGE_SPAN)
		{
			player.charge_span++;
			
		}
	}
	if (GetAsyncKeyState('X') && CHARGE_SPAN <= player.charge_span)
	{
		g_posx = g_posx+= g_velx;
		player.charge_span--;
	}
	
	

}

//#######################################################################
// ゲームの描画関数
void Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
    if( !pDevice ) return;
     
    // 画面のクリア
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(10, 40, 20, 255), 1.0f, 0);

    // 描画バッチ命令の開始
    pDevice->BeginScene();



	//grid.Draw();

	//==============================================
	//ここを追加　テスト用カメラ情報を準備
	//カメラ座標
	g_posCameraEye = D3DXVECTOR3(5, 3, -10.0f);
	g_posCameraAt  = D3DXVECTOR3(0, 1, 0);
	g_vecCameraUp =  D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//ビュー行列の作成
	D3DXMatrixLookAtLH(&g_mtxView,
		&g_posCameraEye, //カメラの視点
		&g_posCameraAt,  //カメラの注視点
		&g_vecCameraUp); //カメラの上方向

	//ビュー行列の設定
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	//==============================================
	// ここを追加　テスト用プロジェクション行列の作成

	D3DXMatrixPerspectiveFovLH(&g_mtxprojection,
		VIEW_ANGEL,  //ビュー平面の視野角
		VIEW_ASPECT, //ビュー平面のアスペクト比
		VIEW_NEAR_Z, //ビュー平面のnearZ値
		VIEW_FAR_Z); //ビュー平面のFarZ値

	//プロジェクション行列の設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxprojection);

	//=============================================================
	//ここを追加　ポリゴンのワールド行列の作成

	//cube1
	
		D3DXMATRIX mtxScl; //スケーリング行列
		D3DXMATRIX mtxRot; //回転行列
		D3DXMATRIX mtxTrs; //平行移動行列

		g_pos = D3DXVECTOR3(g_posx, g_posy, g_posz); //位置
		g_rot = D3DXVECTOR3(0, g_Rotation*5.0f, g_Rotation); //向き(回転)
		g_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //大きさ(スケール)

		D3DXMatrixIdentity(&g_mtxworld);     //ワールド行列を単位行列に初期化

		//スケール行列を作成＆ワールド行列へ合成
		D3DXMatrixScaling(&mtxScl, g_scl.x, g_scl.y, g_scl.z);
		D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxScl); //World * Scaling

		//回転行列を作成＆ワールド行列へ合成
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot.y, g_rot.x, g_rot.z);
		D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxRot); //World * Rotation

		//平行移動行列を作成＆ワールド行列へ合成
		D3DXMatrixTranslation(&mtxTrs, g_pos.x, g_pos.y, g_pos.z);
		D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxTrs); //World * Translation

		//ワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld);

		//描画したいポリゴンの頂点バッファをデータストリーム(データの通り道)にセット
		pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		//描画したいポリゴンの頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//ポリゴンの描画
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_KIZUNA));

		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);
		

	
	


	//D3DPT_LINELIST
	//ここまでを追加　
	//=============================================================  


    // 描画バッチ命令の終了
    pDevice->EndScene();

    // バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
    pDevice->Present(NULL, NULL, NULL, NULL);
}

//#######################################################################
// ゲームの終了処理
void Finalize(void)
{
    // テクスチャの解放
    Texture_Release();

    // Direct3Dラッパーモジュールの終了処理
    MyDirect3D_Finalize();




}