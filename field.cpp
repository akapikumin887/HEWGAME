#include "field.h"

// Field�̏������i�R���X�g���N�^�j
Field::Field()
{
	face = new Face;
}

// Field�̏I�������i�f�X�g���N�^�j
Field::~Field()
{
	delete face;
}

void Field::Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz)
{
	texture_index = tex_idx;
	pos = D3DXVECTOR3(p.x, p.y, p.z);
	rot = D3DXVECTOR3(r.x, r.y, r.z);

	face->CreateFace(sz);
}

void Field::Finalize()
{

}

void Field::Update()
{

}

void Field::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ���[���h�}�g���b�N�X��ݒ�

	face->Draw(texture_index);
}