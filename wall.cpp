#include "wall.h"
#include "camera.h"
#include "aiming.h"

static Wall wall;
static Camera *camera;
static D3DXMATRIX m;

// Wall�̏�����
void Wall_Initialize()
{
	wall.Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(0.0f, 0.0f, 15.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 1.0f), D3DXVECTOR3(10.0f, 0.0f, 20.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

// Wall�̏I������
void Wall_Finalize()
{

}

// Wall�̍X�V
void Wall_Update()
{

	//wall.Rotation_Correction(wall.rot.z += 1.0f);
}

// Wall�̕`��
void Wall_Draw()
{
	wall.Draw_Wall();
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
	//wallv->Sprite_Draw_Billboard(texture_index, pos, scl, revolution, revRadius, revSpd);
	wallv->Sprite_Draw_Billboard(texture_index, pos, scl, revolution, revRadius, revSpd);
	//wallv->Sprite_Draw_FaceEX(texture_index, pos, rot, scl, size, size_nor, revolution, revRadius, revSpd);
}

// Wall�̃Z�b�g
void Wall::Set_Wall(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, D3DXVECTOR3 sz, D3DXVECTOR3 szn, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	//wallv->CreateFaceEX(sz, szn);
	texture_index = textureindex;
	pos = p;
	rot = r;
	scl = s;
	size = sz;
	size_nor = szn;
	revolution = Revolution;
	revRadius = RevRadius;
	revSpd = RevSpd;
}

// Wall�̉�]�̕␳
float Wall::Rotation_Correction(float r)
{
	return wallv->Rotation_Correction(r);
}