#include "game.h"
#include "input.h"
#include "tool_functions.h"
#include "fade.h"

static CameraFP cameraFP;
static Plane plane;
static Aiming2D aiming;
static Target target;

// ����GameObject�̃o�b�t�@����
void Game_Object_Create_Public()
{
	
}

// Game�̏�����
void Game_Initialize()
{
	Game_Object_Create_Public();

	cameraFP.Initialize();
	plane.Initialize(TEXTURE_INDEX_MAX, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 0.0f, 30.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	target.Initialize();
	aiming.Initialize();
	ArrowManager::Initialize(ARROW_MAX);
	Gravility::Initialize();
}

// Game�̏I������
void Game_Finalize()
{
	cameraFP.Finalize();
	plane.Finalize();
	target.Finalize();
	aiming.Finalize();
	ArrowManager::Finalize();
}

// Game�̍X�V
void Game_Update()
{
	// Game�̃��Z�b�g
	if (GetKeyboardTrigger(DIK_RSHIFT))
	{
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
		SetScene(SCENE_TITLE);
		return;
	}

	// CameraFP�̍X�V
	cameraFP.Update();

	// Plane�̍X�V
	plane.Update();

	// Target�̍X�V
	target.Update();
	
	// Aiming3D�̍X�V
	aiming.Update();

	ArrowManager::Update();
}

// Game�̕`��
void Game_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	cameraFP.Draw();
	plane.Draw();
	target.Draw();
	
	ArrowManager::Draw();
	aiming.Draw();
}

// CameraFP���̎擾
CameraFP* Get_Game_CameraFP()
{
	return &cameraFP;
}

// Aiming3D���̎擾
Aiming2D* Get_Game_Aiming()
{
	return &aiming;
}

// Target���̎擾
Target* Get_Game_Target()
{
	return &target;
}