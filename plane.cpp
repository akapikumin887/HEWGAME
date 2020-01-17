#include "plane.h"

// Plane�̏������i�R���X�g���N�^�j
Plane::Plane()
{
	faceEX = new FaceEX;
}

// Plane�̏I�������i�f�X�g���N�^�j
Plane::~Plane()
{
	delete faceEX;
}

// Plane�̏�����
void Plane::Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz, D3DXVECTOR3 szn)
{
	texture_index = tex_idx;
	pos = D3DXVECTOR3(p.x, p.y, p.z);
	rot = D3DXVECTOR3(r.x, r.y, r.z);
	faceEX->CreateFaceEX(sz, szn);
}

// Plane�̏I������
void Plane::Finalize()
{

}

// Plane�̍X�V
void Plane::Update()
{

}

// Plane�̕`��
void Plane::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ���[���h�}�g���b�N�X��ݒ�

	faceEX->Draw(texture_index);
}