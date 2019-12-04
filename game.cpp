#include "game.h"
#include "input.h"
#include "ui.h"
#include "gravility.h"
#include "wind.h"
#include "cube.h"
#include "plane.h"
#include "wall.h"
#include "target.h"
#include "aiming.h"

// Gameの初期化
void Game_Initialize()
{
	CreateBillboard();
	CreateFace();
	CreateCube();
	UI_Initialize();
	Gravility_Initialize();
	Wind_Initialize();
	Cube_Initialize();
	Plane_Initialize();
	//Wall_Initialize();
	Target_Initialize();
	Aiming_Initialize();
}

// Gameの終了処理
void Game_Finalize()
{
	UI_Finalize();
	Gravility_Finalize();
	Wind_Finalize();
	Cube_Finalize();
	Plane_Finalize();
	//Wall_Finalize();
	Target_Finalize();
	Aiming_Finalize();
}

// Gameの更新
void Game_Update()
{
	// Gameのリセット
	if (Keyboard_IsTrigger(DIK_RSHIFT))
	{
		SetScene(SCENE_GAME);
		return;
	}
	UI_Update();
	Gravility_Update();
	Wind_Update();
	Cube_Update();
	Plane_Update();
	//Wall_Update();
	Target_Update();
	Aiming_Update();
	
}

// Gameの描画
void Game_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	UI_Draw();
	Gravility_Draw();
	Wind_Draw();
	Cube_Draw();
	Plane_Draw();
	//Wall_Draw();
	Target_Draw();
	Aiming_Draw();
}