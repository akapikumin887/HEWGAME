#include "light.h"

// �O���[�o���ϐ�
D3DLIGHT9 g_Light; // ���C�g�\����

// Light�̏�����
void Light_Initialize()
{
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9)); // �\���̂�0�ŃN���A�i�K�{�j
	g_Light.Type = D3DLIGHT_DIRECTIONAL; // ���C�g�̎��
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // ���̐FRGBA�i0�`1�j
	g_Light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // �����̐F

	D3DXVECTOR3 vecDir = D3DXVECTOR3(0.0f, 1.0f, -1.0f); // ���̍��������x�N�g��
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_Light.Direction = vecDir; // ���K�����ăZ�b�g

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->SetLight(0, &g_Light); // 0�ԃ��C�g�փf�[�^���Z�b�g
	pDevice->LightEnable(0, TRUE); // 0�ԃ��C�g��L����
}

// Light�̏I������
void Light_Finalize()
{
	
}

// Light�̍X�V
void Light_Update()
{
	
}

// Light�̕`��
void Light_Draw()
{

}