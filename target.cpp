#include "target.h"
#include "tool_functions.h"

// Target�̏������i�R���X�g���N�^�j
Target::Target()
{
	face = new Face;
}

// Target�̏I�������i�f�X�g���N�^�j
Target::~Target()
{
	delete face;
}

void Target::Initialize(TextureIndex tex_idx, D3DXVECTOR3 p, D3DXVECTOR3 r, D3DXVECTOR3 sz)
{
	texture_index = tex_idx;
	pos = D3DXVECTOR3(p.x, p.y, p.z);
	rot = D3DXVECTOR3(r.x, r.y, r.z);
	
	face->CreateFace(sz);

	// �e�̔��a��������
	float c = TARGET_SIZE / 20;
	circle[0] = c / 2;
	for (int i = 1; i < 11; i++)
	{
		circle[i] = i * c;
		if (i == 8)
		{
			circle[i] -= c / 5;
		}
	}
}

void Target::Finalize()
{
	
}

void Target::Update()
{

}

void Target::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	mtxWorld = Set_Mtx_Scl(mtxWorld);
	mtxWorld = Set_Mtx_Rot(mtxWorld, rot);
	mtxWorld = Set_Mtx_Trs(mtxWorld, pos);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); // ���[���h�}�g���b�N�X��ݒ�

	face->Draw(texture_index);
}