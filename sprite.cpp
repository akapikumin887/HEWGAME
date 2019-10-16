#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "common.h"
#include "mydirect3d.h"
#include "debug_font.h"
#include "sprite.h"
#include "texture.h"

// 2D�|���S���̒��_�\����
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position; // ���_���W�i���W�ϊ��ςݒ��_�j
	D3DCOLOR color; // ���_�J���[(32Bit ARGB�w��)
	D3DXVECTOR2 texcoord; // �e�N�X�`�����W
}Vertex2D;

// 3D�|���S���̒��_�\����
typedef struct Vertex3D_tag
{
	D3DXVECTOR3 vtx; // ���_���W
	D3DXVECTOR3 nor; // �@��
	D3DXCOLOR diffuse; // ���ˌ��i���_�F�j
	D3DXVECTOR2 tex; // �e�N�X�`�����W
}VERTEX_3D;

// �O���[�o���ϐ�
static D3DCOLOR g_Color = 0xffffffff; // ���s����
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

//====================================================
// ������
//====================================================
void Sprite_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	// �o�[�e�b�N�X�o�b�t�@���쐬����
	pDevice->CreateVertexBuffer(sizeof(Vertex2D) * 4, // 4���_�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX2D,
		D3DPOOL_DEFAULT,
		&g_pVertexBuffer,
		NULL);

	// �C���f�b�N�X�o�b�t�@���g��
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, // �C���f�b�N�X6���Ԃ�
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&g_pIndexBuffer,
		NULL);

	// �C���f�b�N�X�ɏ�������ł���
	LPWORD pIndex = NULL;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0); // �C���f�b�N�X�z��̃|�C���^�����炤
	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 1;
	pIndex[4] = 3;
	pIndex[5] = 2;
	g_pIndexBuffer->Unlock(); // �`�����ݏI��
}

//====================================================
// �X�v���C�g�\��
//====================================================
void Sprite_Draw(TextureIndex texture_index,float dx, float dy, int tx,int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// �e�N�X�`�����W�v�Z
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx+tw) / w;
	v[1] = (float)(ty + th) / h; // �s�N�Z�����W���e�N�X�`�����W�ɕϊ�

	Vertex2D* pv; // ���_�z��̃|�C���^�̓��ꕨ

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // �z��̃|�C���^���󂯎��

	// ���_�f�[�^��ݒ�
	pv[0].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}
	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock(); // �o�[�e�b�N�X�o�b�t�@�ւ̏������ݏI��

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X
	
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // ���_��
		0, 2); // 2���|���S���̐�
}

void Sprite_Draw_Rotation(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}
	float w = (float)Texture_GetWidth(texture_index);
	float h = (float)Texture_GetHeight(texture_index);

	// �s��ϐ��錾
	D3DXMATRIX world; // ���[���h�s��
	D3DXMATRIX rotation; // ��]�s��
	D3DXMATRIX translation; // ���s�ړ��s��

	D3DXMatrixTranslation(&translation, dx, dy, 0.0f); // ���s�ړ��s��쐬
	D3DXMatrixRotationZ(&rotation, radian); // radian����������]����s����쐬
	D3DXMatrixMultiply(&world, &rotation, &translation); // world = rotation * translation

	// �e�N�X�`�����W�v�Z
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = (float)(tx + tw) / w;
	v[1] = (float)(ty + th) / h; // �s�N�Z�����W���e�N�X�`�����W�ɕϊ�
	
	Vertex2D* pv;

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // �z��̃|�C���^���󂯎��

	// ���_�f�[�^��ݒ�
	pv[0].position = D3DXVECTOR4(
		-lx - 0.5f,
		-ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		+lx - 0.5f,
		-ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		-lx - 0.5f,
		+ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		+lx - 0.5f,
		+ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}

	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	// ���_���W��ϊ�����
	for (int i = 0; i < 4; i++)
	{
		D3DXVec4Transform(
			&pv[i].position, // �ϊ����ʂ̊i�[��|�C���^
			&pv[i].position, // �ϊ��O�f�[�^�̃|�C���^
			&world); // �ϊ��Ɏg���s��̃|�C���^
	}

	g_pVertexBuffer->Unlock(); // �o�[�e�b�N�X�o�b�t�@�ւ̏������ݏI��

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pv, sizeof(Vertex2D));
}

//====================================================
// BG�p�X�v���C�g�`��
// �@tx, ty �e�N�X�`��������W�@�@tw, th �\���X�v���C�g�T�C�Y
//====================================================
void Sprite_DrawEx(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th)
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}

	float w = (float)Texture_GetWidth(texture_index);//�e�N�X�`���T�C�Y
	float h = (float)Texture_GetHeight(texture_index);

	// �e�N�X�`�����W�v�Z
	float u[2], v[2];
	u[0] = (float)tx / w;
	v[0] = (float)ty / h;
	u[1] = u[0] + 1.0f;
	v[1] = v[0] + 1.0f; // �s�N�Z�����W���e�N�X�`�����W�ɕϊ�

	Vertex2D* pv; // ���_�z��̃|�C���^�̓��ꕨ

	float	lx = tw / 2.0f;
	float	ly = th / 2.0f;

	g_pVertexBuffer->Lock(0, 0, (void**)&pv, 0); // �z��̃|�C���^���󂯎��

    // ���_�f�[�^��ݒ�
	pv[0].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[1].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy - ly - 0.5f,
		0.0,
		1.0f);
	pv[2].position = D3DXVECTOR4(
		dx - lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);
	pv[3].position = D3DXVECTOR4(
		dx + lx - 0.5f,
		dy + ly - 0.5f,
		0.0,
		1.0f);

	for (int i = 0; i < 4; i++)
	{
		pv[i].color = g_Color;
	}
	pv[0].texcoord = D3DXVECTOR2(u[0], v[0]);
	pv[1].texcoord = D3DXVECTOR2(u[1], v[0]);
	pv[2].texcoord = D3DXVECTOR2(u[0], v[1]);
	pv[3].texcoord = D3DXVECTOR2(u[1], v[1]);

	g_pVertexBuffer->Unlock(); // �o�[�e�b�N�X�o�b�t�@�ւ̏������ݏI��

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // ���_��
		0, 2); // 2���|���S���̐�
}

void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Finalize(void)
{
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}