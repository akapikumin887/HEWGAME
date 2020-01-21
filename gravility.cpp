#include "gravility.h"
#include "mode.h"

D3DXVECTOR3 Gravility::g;

// Gravility�̏�����
void Gravility::Initialize()
{
	if (Get_Mode() != MODE_EASY)
	{
		g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
	}
	else
	{
		g = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

// Gravility�̏I������
void Gravility::Finalize()
{

}

// Gravility�̍X�V
void Gravility::Update()
{

}

// Gravility�̕`��
void Gravility::Draw()
{

}