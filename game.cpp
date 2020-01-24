#include "game.h"
#include "input.h"
#include "debug_font.h"
#include "tool_functions.h"
#include "fade.h"
#include "mode.h"

static CameraFP cameraFP;
static Plane plane;
static Aiming2D aiming;
static Target target;
static Mode mode;
static Field field[FIELD_MAX];
static Number sn;
static Alphabet sa;

// 共通GameObjectのバッファ生成
void Game_Object_Create_Public()
{
	
}

// Gameの初期化
void Game_Initialize()
{
	Game_Object_Create_Public();
	//Set_Mode(MODE_HARD);
	cameraFP.Initialize(D3DXVECTOR3(CAMERAEYE_X, CAMERAEYE_Y, CAMERAEYE_Z), D3DXVECTOR3(CAMERAAT_X, CAMERAAT_Y, CAMERAAT_Z));
	plane.Initialize(TEXTURE_INDEX_PLANE, D3DXVECTOR3(0.0f, -2.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(32.0f, 0.0f, 48.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	
	field[0].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(-16.0f, 2.0f, -16.0f), D3DXVECTOR3(-90.0f, -90.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));
	field[1].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(-16.0f, 2.0f, 0.0f), D3DXVECTOR3(-90.0f, -90.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));
	field[2].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(-16.0f, 2.0f, 16.0f), D3DXVECTOR3(-90.0f, -90.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));

	field[3].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(16.0f, 2.0f, -16.0f), D3DXVECTOR3(-90.0f, 90.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));
	field[4].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(16.0f, 2.0f, 0.0f), D3DXVECTOR3(-90.0f, 90.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));
	field[5].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(16.0f, 2.0f, 16.0f), D3DXVECTOR3(-90.0f, 90.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));

	field[6].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(-8.0f, 2.0f, 24.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));
	field[7].Initialize(TEXTURE_INDEX_SIDE, D3DXVECTOR3(8.0f, 2.0f, 24.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(16.0f, 0.0f, 9.0f));
	
	target.Initialize();
	aiming.Initialize();
	ArrowManager::Initialize(ARROW_MAX);
	Gravility::Initialize();

	sn.Initialize(TEXTURE_INDEX_NUMBER, D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(SCREEN_WIDTH, 32.0f), 0, 0, 10, 1, SCORE_DIGIT_MAX);
	sa.Initialize(TEXTURE_INDEX_ALPHABET, "SCORE", D3DXVECTOR2(32.0f, 32.0f), D3DXVECTOR2(sn.pos.x - sn.digit_i * sn.tw, 32.0f), 0, 0, 13, 2, 2);
}

// Gameの終了処理
void Game_Finalize()
{
	cameraFP.Finalize();
	plane.Finalize();
	target.Finalize();
	aiming.Finalize();
	sn.Finalize();
	sa.Finalize();
}

// Gameの更新
void Game_Update()
{
	// Gameのリセット
	if (GetKeyboardTrigger(DIK_RSHIFT))
	{
		ArrowManager::Finalize();
		SetScene(SCENE_GAME);
		return;
	}
	
	if (GetKeyboardTrigger(DIK_0))
	{
		Fade(SCENE_RANKING);
		return;
	}

	if (ArrowManager::cnt == ARROW_MAX && GetKeyboardTrigger(DIK_C) && aiming.state == AIMING_STATE_FREE || GetKeyboardTrigger(DIK_T))
	{
		//SetScene(SCENE_TITLE);
		SetScene(SCENE_RESULT);
		return;
	}

	// CameraFPの更新
	cameraFP.Update();

	// Planeの更新
	plane.Update();

	// Targetの更新
	target.Update();
	
	// Aiming3Dの更新
	aiming.Update();

	ArrowManager::Update();

	sn.Update();
	sa.Update();
}

// Gameの描画
void Game_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	cameraFP.Draw();
	plane.Draw();

	for (int i = 0; i < FIELD_MAX; i++)
	{
		field[i].Draw();
	}

	target.Draw();
	
	ArrowManager::Draw();

	aiming.Draw();

	sn.Draw(ArrowManager::score_t, sn.digit_i);
	sa.Draw();

	//DebugFont_Draw(2, 2, "CameraAtRot  x: %.2lf  y: %.2lf  z: %.2lf", cameraTP.rotAt.x, cameraTP.rotAt.y, cameraTP.rotAt.z);
	DebugFont_Draw(2, 2, "CameraEye  x: %.2lf  y: %.2lf  z: %.2lf", cameraFP.posEye.x, cameraFP.posEye.y, cameraFP.posEye.z);
	DebugFont_Draw(2, 32, "CameraAt  x: %.2lf  y: %.2lf  z: %.2lf", cameraFP.posAt.x, cameraFP.posAt.y, cameraFP.posAt.z);
	DebugFont_Draw(2, 62, "CameraEyeRot  x: %.2lf  y: %.2lf  z: %.2lf", cameraFP.rotEye.x, cameraFP.rotEye.y, cameraFP.rotEye.z);
	DebugFont_Draw(2, 272, "score  x: %d", ArrowManager::score_t);
}

// CameraFP情報の取得
CameraFP* Get_Game_CameraFP()
{
	return &cameraFP;
}

// Aiming3D情報の取得
Aiming2D* Get_Game_Aiming()
{
	return &aiming;
}

// Target情報の取得
Target* Get_Game_Target()
{
	return &target;
}

// Plane情報の取得
Plane* Get_Game_Plane()
{
	return &plane;
}