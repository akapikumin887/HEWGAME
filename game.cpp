#include "game.h"
#include "input.h"
#include "aiming.h"
#include "cube.h"
#include "plane.h"
#include "wall.h"
#include "target.h"

// Game�̏�����
void Game_Initialize()
{
	CreateFace();
	CreateCube();
	Aiming_Initialize();
	Cube_Initialize();
	Plane_Initialize();
	//Wall_Initialize();
	Target_Initialize();
}

// Game�̏I������
void Game_Finalize()
{
	Aiming_Finalize();
	Cube_Finalize();
	Plane_Finalize();
	//Wall_Finalize();
	Target_Finalize();
}

// Game�̍X�V
void Game_Update()
{
	// Game�̃��Z�b�g
	if (Keyboard_IsTrigger(DIK_RSHIFT))
	{
		SetScene(SCENE_GAME);
		return;
	}
	Aiming_Update();
	Cube_Update();
	Plane_Update();
	//Wall_Update();
	Target_Update();
}

// Game�̕`��
void Game_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	Cube_Draw();
	Plane_Draw();
	//Wall_Draw();
	Target_Draw();
	Aiming_Draw();
}