#include "wall.h"
#include "aiming.h"

static Wall target[3];

// Wall�̏�����
void Wall_Initialize()
{
	target[0].Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(5.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 90.0f), D3DXVECTOR3(10.0f, 1.0f, 10.0f));
	target[1].Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(-5.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 270.0f), D3DXVECTOR3(10.0f, 1.0f, 10.0f));
	target[2].Set_Wall(TEXTURE_INDEX_AIROU, D3DXVECTOR3(AIMING_X, AIMING_Y, AIMING_Z * AIMING_MAG_Z), D3DXVECTOR3(270.0f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 1.0f, 5.0f));
}

// Wall�̏I������
void Wall_Finalize()
{

}

// Wall�̍X�V
void Wall_Update()
{
	target[0].Rotation_Correction(target[0].rot.z += 1.0f);
}

// Wall�̕`��
void Wall_Draw()
{
	//wall[0].Draw_Wall();
	//wall[1].Draw_Wall();
	target[2].Draw_Wall();
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