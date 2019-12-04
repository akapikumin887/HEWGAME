#include "plane.h"

static Plane plane;

// Plane�̏�����
void Plane_Initialize()
{
	plane.Set_Plane(TEXTURE_INDEX_MAX, D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(100.0f, 0.0f, 200.0f), D3DXVECTOR3(0.5f, 0.5f, 0.5f));
}

// Plane�̏I������
void Plane_Finalize()
{

}

// Plane�̍X�V
void Plane_Update()
{
	
}

// Plane�̕`��
void Plane_Draw()
{
	plane.Draw_Plane();
}

// Plane�̒��_���擾�i�R���X�g���N�^�j
Plane::Plane()
{
	planev = new VERTEX_3D;
}

// Plane�̒��_���̃����[�X�i�f�X�g���N�^�j
Plane::~Plane()
{
	delete planev;
}

// Plane�̕`��
void Plane::Draw_Plane()
{
	planev->Sprite_Draw_FaceEX(texture_index, pos, rot, scl, size, size_nor,revolution, revRadius, revSpd);
}

// Plane�̃Z�b�g
void Plane::Set_Plane(TextureIndex textureindex, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 s, D3DXVECTOR3 sz, D3DXVECTOR3 szn, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	planev->CreateFaceEX(sz, szn);
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

// Plane�̉�]�̕␳
float Plane::Rotation_Correction(float r)
{
	return planev->Rotation_Correction(r);
}