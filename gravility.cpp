#include "gravility.h"

D3DXVECTOR3 Gravility::g;

// Gravility�̏�����
void Gravility::Initialize()
{
	g = D3DXVECTOR3(0.0f, GRAVILITY, 0.0f) / GRAVILITY_FORCE_MAG;
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