#include "wall.h"
#include "input.h"
#include "camera.h"
#include "aiming.h"

static Wall wall;
static Camera camera;

// Wall�̏�����
void Wall_Initialize()
{
	wall.Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(0.0f, 0.0f, 15.0f), D3DXVECTOR3(-90.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 1.0f, 10.0f));
	camera.Initialize(wall.pos);
}

// Wall�̏I������
void Wall_Finalize()
{
	camera.Finalize(wall.pos);
}

// Wall�̍X�V
void Wall_Update()
{
	if (GetKeyboardPress(DIK_W))
	{
		camera.posAt.x = wall.pos.x += sinf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
		camera.posAt.z = wall.pos.z += cosf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
	}
	if (GetKeyboardPress(DIK_S))
	{
		camera.posAt.x = wall.pos.x -= sinf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
		camera.posAt.z = wall.pos.z -= cosf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
	}

	if (GetKeyboardPress(DIK_A))
	{
		camera.posAt.x = wall.pos.x -= cosf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
		camera.posAt.z = wall.pos.z += sinf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
	}
	if (GetKeyboardPress(DIK_D))
	{
		camera.posAt.x = wall.pos.x += cosf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
		camera.posAt.z = wall.pos.z -= sinf(D3DXToRadian(camera.rotAt.y)) * WALLMOVE;
	}
	if (GetKeyboardPress(DIK_Q))
	{
		wall.rot.y -= 1.0f;
		wall.Rotation_Correction(wall.rot.y);
	}
	if (GetKeyboardPress(DIK_E))
	{
		wall.rot.y += 1.0f;
		wall.Rotation_Correction(wall.rot.y);
	}
	camera.CameraAt_Rot();
}

// Wall�̕`��
void Wall_Draw()
{
	wall.Draw_Wall();
	camera.Draw();
}

// Wall�̒��_���擾�i�R���X�g���N�^�j
Wall::Wall()
{
	wallv = new VERTEX_3D;
}

// Wall�̒��_���̃����[�X�i�f�X�g���N�^�j
Wall::~Wall()
{
	delete wallv;
}

// Wall�̕`��
void Wall::Draw_Wall()
{
	wallv->Sprite_Draw_Face(texture_index, pos, rot, scl, revolution, revRadius, revSpd);
}

// Wall�̃Z�b�g
void Wall::Set_Wall(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	texture_index = textureindex;
	pos = p;
	rot = r;
	scl = s;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;
}

// Wall�̉�]�̕␳
float Wall::Rotation_Correction(float r)
{
	return wallv->Rotation_Correction(r);
}