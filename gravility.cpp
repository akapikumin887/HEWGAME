#include "gravility.h"

static Gravility gravility;

// Gravility�̏�����
void Gravility_Initialize()
{
	gravility.g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
}

// Gravility�̏I������
void Gravility_Finalize()
{

}

// Gravility�̍X�V
void Gravility_Update()
{

}

// Gravility�̕`��
void Gravility_Draw()
{

}

// Gravility���̎擾
Gravility* Get_Gravility()
{
	return &gravility;
}