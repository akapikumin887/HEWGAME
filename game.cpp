#include "game.h"
#include "input.h"
#include "camera.h"
#include "ui.h"
#include "gravility.h"
#include "wind.h"
#include "cube.h"
#include "plane.h"
#include "wall.h"
#include "target.h"
#include "aiming.h"
#include "particle.h"

//static PARTICLE particle;

// Gameの初期化
void Game_Initialize()
{
	Camera_Initialize();
	CreateFace();
	//CreateBillboard();
	CreateCube();
	Gravility_Initialize();
	Wind_Initialize();
	Cube_Initialize();
	Plane_Initialize();
	Target_Initialize();
	Aiming_Initialize();
	UI_Initialize();

	/*Plane_Initialize();
	Wall_Initialize();*/
	//particle.Init(100);
}

// Gameの終了処理
void Game_Finalize()
{
	Camera_Finalize();
	Gravility_Finalize();
	Wind_Finalize();
	Cube_Finalize();
	Plane_Finalize();
	Target_Finalize();
	Aiming_Finalize();
	UI_Finalize();

	/*Plane_Finalize();
	Wall_Finalize();*/
	//particle.Uninit();
}

// Gameの更新
void Game_Update()
{
	// Gameのリセット
	if (GetKeyboardTrigger(DIK_RSHIFT))
	{
		SetScene(SCENE_GAME);
		return;
	}
	if (GetKeyboardTrigger(DIK_0))
	{
		SetScene(SCENE_RANKING);
		return;
	}
	Camera_Update();
	Gravility_Update();
	Wind_Update();
	Cube_Update();
	Plane_Update();
	Target_Update();
	Aiming_Update();
	UI_Update();

	//Plane_Update();
	//Wall_Update();
	//particle.Update();
}

// Gameの描画
void Game_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	Camera_Draw();
	Gravility_Draw();
	Wind_Draw();
	Cube_Draw();
	Plane_Draw();
	Target_Draw();
	Aiming_Draw();
	UI_Draw();

	//Plane_Draw();
	//Wall_Draw();
	//particle.Draw();
}