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
#include "camera.h"

// �O���[�o���ϐ�
static D3DCOLOR g_Color = 0xffffffff; // ���s����
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static D3DXMATRIX g_mtxGWorld;

static Camera *camera;

LPDIRECT3DVERTEXBUFFER9 VERTEX_3D::pD3DVtxBufferBillboard = NULL; // Billboard���_�o�b�t�@
LPDIRECT3DINDEXBUFFER9 VERTEX_3D::pD3DIdxBufferBillboard = NULL; // Billboard���_�C���f�b�N�X
LPDIRECT3DVERTEXBUFFER9 VERTEX_3D::pD3DVtxBufferFace = NULL; // Face���_�o�b�t�@
LPDIRECT3DINDEXBUFFER9 VERTEX_3D::pD3DIdxBufferFace = NULL; // Face���_�C���f�b�N�X
LPDIRECT3DVERTEXBUFFER9 VERTEX_3D::pD3DVtxBufferCube = NULL; // Cube���_�o�b�t�@

//====================================================
// ������
//====================================================
void Sprite_Initialize_2D(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}
	// �o�[�e�b�N�X�o�b�t�@���쐬����
	pDevice->CreateVertexBuffer(sizeof(Vertex_2D) * 4, // 4���_�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
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
void Sprite_Draw_2D(TextureIndex texture_index,float dx, float dy, int tx,int ty, int tw, int th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

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

	Vertex_2D* pv; // ���_�z��̃|�C���^�̓��ꕨ

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

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex_2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X
	
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // ���_��
		0, 2); // 2���|���S���̐�
}

void Sprite_Draw_Rotation_2D(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float radian)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

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
	
	Vertex_2D* pv;

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

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex_2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pv, sizeof(Vertex_2D));
}

//====================================================
// BG�p�X�v���C�g�`��
// �@tx, ty �e�N�X�`��������W�@�@tw, th �\���X�v���C�g�T�C�Y
//====================================================
void Sprite_DrawEx_2D(TextureIndex texture_index, float dx, float dy, float tx, float ty, float tw, float th)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

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

	Vertex_2D* pv; // ���_�z��̃|�C���^�̓��ꕨ

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

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->SetStreamSource(0,
		g_pVertexBuffer, // �`�悵�����|���S���̃o�[�e�b�N�X�o�b�t�@
		0,
		sizeof(Vertex_2D)); // ���_1���̃T�C�Y

	pDevice->SetIndices(g_pIndexBuffer); // �`�悵�����|���S���̃C���f�b�N�X

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
		0, 0,
		4, // ���_��
		0, 2); // 2���|���S���̐�
}

void Sprite_SetColor_2D(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Finalize_2D(void)
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

void Sprite_Finalize_3D()
{
	if (VERTEX_3D::pD3DVtxBufferBillboard)
	{
		VERTEX_3D::pD3DVtxBufferBillboard->Release();
		VERTEX_3D::pD3DVtxBufferBillboard = NULL;
	}

	if (VERTEX_3D::pD3DIdxBufferBillboard != NULL)
	{
		VERTEX_3D::pD3DIdxBufferBillboard->Release(); // �C���^�[�t�F�[�X�̉��
		VERTEX_3D::pD3DIdxBufferBillboard = NULL;
	}

	if (VERTEX_3D::pD3DVtxBufferFace)
	{
		VERTEX_3D::pD3DVtxBufferFace->Release();
		VERTEX_3D::pD3DVtxBufferFace = NULL;
	}

	if (VERTEX_3D::pD3DIdxBufferFace != NULL)
	{
		VERTEX_3D::pD3DIdxBufferFace->Release(); // �C���^�[�t�F�[�X�̉��
		VERTEX_3D::pD3DIdxBufferFace = NULL;
	}

	if (VERTEX_3D::pD3DVtxBufferCube)
	{
		VERTEX_3D::pD3DVtxBufferCube->Release();
		VERTEX_3D::pD3DVtxBufferCube = NULL;
	}
}

// Billboard�i�o�b�t�@�j�̐���
void CreateBillboard()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY,   // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,        // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,      // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&VERTEX_3D::pD3DVtxBufferBillboard, // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);                // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VERTEX_3D::pD3DVtxBufferBillboard->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
	
	for (int i = 0; i < 4; i++)
	{
		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���ˌ��̐ݒ�
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	VERTEX_3D::pD3DVtxBufferBillboard->Unlock();

	// �X�g���b�v�p
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&VERTEX_3D::pD3DIdxBufferBillboard,
		NULL);

	WORD* pIdx = NULL; // �z��̐擪�|�C���^�̓��ꕨ

	VERTEX_3D::pD3DIdxBufferBillboard->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 3;

	VERTEX_3D::pD3DIdxBufferBillboard->Unlock();
}

// Face�i�o�b�t�@�j�̐���
void CreateFace()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY,   // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,        // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,      // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&VERTEX_3D::pD3DVtxBufferFace, // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);                // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VERTEX_3D::pD3DVtxBufferFace->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
	pVtx[1].vtx = D3DXVECTOR3(0.5f, 0.0f, 0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-0.5f, 0.0f, -0.5f);
	pVtx[3].vtx = D3DXVECTOR3(0.5f, 0.0f, -0.5f);

	for (int i = 0; i < 4; i++)
	{
		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���ˌ��̐ݒ�
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	VERTEX_3D::pD3DVtxBufferFace->Unlock();

	// �X�g���b�v�p
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&VERTEX_3D::pD3DIdxBufferFace,
		NULL);

	WORD* pIdx = NULL; // �z��̐擪�|�C���^�̓��ꕨ

	VERTEX_3D::pD3DIdxBufferFace->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 3;

	VERTEX_3D::pD3DIdxBufferFace->Unlock();
}

// Cube�i�o�b�t�@�j�̐���
void CreateCube()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 36, // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY,   // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,        // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,      // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&VERTEX_3D::pD3DVtxBufferCube, // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);                // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VERTEX_3D::pD3DVtxBufferCube->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx[1].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[2].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	pVtx[3].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[4].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx[5].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);

	pVtx[6].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[7].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[8].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);

	pVtx[9].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[10].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[11].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);

	pVtx[12].vtx = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtx[13].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[14].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	pVtx[15].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[16].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx[17].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	pVtx[18].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtx[19].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[20].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);

	pVtx[21].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[22].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVtx[23].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);

	pVtx[24].vtx = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtx[25].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx[26].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	pVtx[27].vtx = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtx[28].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx[29].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	pVtx[30].vtx = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtx[31].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx[32].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	pVtx[33].vtx = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtx[34].vtx = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtx[35].vtx = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);

	for (int i = 0; i < 36; i++)
	{
		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���ˌ��̐ݒ�
		pVtx[i].diffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

	for (int i = 0; i < 36; i += 6)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[i + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[i + 3].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i + 4].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[i + 5].tex = D3DXVECTOR2(0.0f, 1.0f);
	}
	//���_�f�[�^���A�����b�N����
	VERTEX_3D::pD3DVtxBufferCube->Unlock();
}

VERTEX_3D::VERTEX_3D()
{
	pD3DVtxBufferFaceEX = NULL;
	pD3DIdxBufferFaceEX = NULL;
}

VERTEX_3D::~VERTEX_3D()
{
	if (pD3DVtxBufferFaceEX)
	{
		pD3DVtxBufferFaceEX->Release();
		pD3DVtxBufferFaceEX = NULL;
	}

	if (pD3DIdxBufferFaceEX != NULL)
	{
		pD3DIdxBufferFaceEX->Release(); // �C���^�[�t�F�[�X�̉��
		pD3DIdxBufferFaceEX = NULL;
	}
}

void VERTEX_3D::CreateFaceEX(D3DXVECTOR3 sz, D3DXVECTOR3 szn)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	int kx = (int)(sz.x / szn.x); // ���i��
	int kz = (int)(sz.z / szn.z); // �c�i��

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((kx + 1) * (kz + 1)), // ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�i�o�C�g�P�ʁj
		D3DUSAGE_WRITEONLY,     // ���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,          // �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,        // ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&pD3DVtxBufferFaceEX, // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);                  // NULL�ɐݒ�

	// ���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	pD3DVtxBufferFaceEX->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < (kx + 1) * (kz + 1); i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i].vtx = D3DXVECTOR3((i % (kx + 1)) * szn.x - sz.x / 2.0f, 0.0f, sz.z / 2.0f - i / (kx + 1) * szn.z);

		// �@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���ˌ��̐ݒ�
		pVtx[i].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�̐ݒ�
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^���A�����b�N����
	pD3DVtxBufferFaceEX->Unlock();

	// �X�g���b�v�p
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((3 + (kz * 2) - 1) * kx + 2 * (kx - 1)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pD3DIdxBufferFaceEX,
		NULL);

	WORD* pIdx = NULL; // �z��̐擪�|�C���^�̓��ꕨ

	pD3DIdxBufferFaceEX->Lock(0, 0, (void**)&pIdx, 0);

	int count = 0;

	for (int i = 0; i < kx; i++)
	{
		for (int j = 0; j < kz + 1; j++)
		{
			pIdx[count] = i + j * (kx + 1);
			pIdx[count + 1] = pIdx[count] + 1;
			count += 2;
		}
		pIdx[count] = pIdx[count - 1];
		pIdx[count + 1] = pIdx[count - (kz + 1) * 2 + 1];
	}
	pD3DIdxBufferFaceEX->Unlock();
}

// Billboard�̕`��
void VERTEX_3D::Sprite_Draw_Billboard(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 scl, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxScl;    // �X�P�[�����O�s��
	D3DXMATRIX mtxCamera; // Camera�̓]�u�i��]�s��j
	D3DXMATRIX mtxTrs;    // ���s�ړ��s��

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	// �X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl); // World * Scaling

	// ��]�s����쐬�����[���h�s��֍���
	camera = Get_Camera(); // Camera���̎擾
	mtxCamera = camera->mtxView; // Camera�s��̊i�[
	mtxCamera._41 = 0.0f; // Camera�̕��s�ړ��s���0��
	mtxCamera._42 = 0.0f;
	mtxCamera._43 = 0.0f;
	D3DXMatrixTranspose(&mtxCamera, &mtxCamera); // Camera�s��̓]�u
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxCamera); // World * Rotation

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, VERTEX_3D::pD3DVtxBufferBillboard, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(VERTEX_3D::pD3DIdxBufferBillboard);

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

// Face�̕`��
void VERTEX_3D::Sprite_Draw_Face(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxScl; // �X�P�[�����O�s��
	D3DXMATRIX mtxRot; // ��]�s��
	D3DXMATRIX mtxTrs; // ���s�ړ��s��

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	// �X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl); // World * Scaling

	// ��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y), D3DXToRadian(rot.x), D3DXToRadian(rot.z));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation

	// ���]
	if (Revolution)
	{
		// ���s�ړ��s����쐬�����[���h�s��֍���
		D3DXMatrixTranslation(&mtxTrs, RevRadius.x, RevRadius.y, RevRadius.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

		// ��]�s����쐬�����[���h�s��֍���
		D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(RevSpd.y), D3DXToRadian(RevSpd.x), D3DXToRadian(RevSpd.z));
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation
	}

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, VERTEX_3D::pD3DVtxBufferFace, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(VERTEX_3D::pD3DIdxBufferFace);

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2);
}

void VERTEX_3D::Sprite_Draw_FaceEX(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, D3DXVECTOR3 size, D3DXVECTOR3 size_nor, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxScl; // �X�P�[�����O�s��
	D3DXMATRIX mtxRot; // ��]�s��
	D3DXMATRIX mtxTrs; // ���s�ړ��s��
	int kx = (int)(size.x / size_nor.x);
	int kz = (int)(size.z / size_nor.z);

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	// �X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl); // World * Scaling

	// ��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y), D3DXToRadian(rot.x), D3DXToRadian(rot.z));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation

	// ���]
	if (Revolution)
	{
		// ���s�ړ��s����쐬�����[���h�s��֍���
		D3DXMatrixTranslation(&mtxTrs, RevRadius.x, RevRadius.y, RevRadius.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

		// ��]�s����쐬�����[���h�s��֍���
		D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(RevSpd.y), D3DXToRadian(RevSpd.x), D3DXToRadian(RevSpd.z));
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation
	}

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, pD3DVtxBufferFaceEX, 0, sizeof(VERTEX_3D));
	pDevice->SetIndices(pD3DIdxBufferFaceEX);

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// ���u�����h���s��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ((3 + (kz * 2) - 1) * kx + 2 * (kx - 1)), 0, kz * kx * 2 + (kx - 1) * 2);
}

// Cube�̕`��
void VERTEX_3D::Sprite_Draw_Cube(TextureIndex texture_index, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl, bool Revolution, D3DXVECTOR3 RevRadius, D3DXVECTOR3 RevSpd)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxScl; // �X�P�[�����O�s��
	D3DXMATRIX mtxRot; // ��]�s��
	D3DXMATRIX mtxTrs; // ���s�ړ��s��

	D3DXMatrixIdentity(&mtxWorld); // ���[���h�s���P�ʍs��ɏ�����

	// �X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl); // World * Scaling

	// ��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(rot.y), D3DXToRadian(rot.x), D3DXToRadian(rot.z));
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation

	// ���]
	if (Revolution)
	{
		// ���s�ړ��s����쐬�����[���h�s��֍���
		D3DXMatrixTranslation(&mtxTrs, RevRadius.x, RevRadius.y, RevRadius.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

		// ��]�s����쐬�����[���h�s��֍���
		D3DXMatrixRotationYawPitchRoll(&mtxRot, D3DXToRadian(RevSpd.y), D3DXToRadian(RevSpd.x), D3DXToRadian(RevSpd.z));
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot); // World * Rotation
	}

	// ���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrs); // World * Translation

	// ���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// �`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[���i�f�[�^�̒ʂ蓹�j�ɃZ�b�g
	pDevice->SetStreamSource(0, VERTEX_3D::pD3DVtxBufferCube, 0, sizeof(VERTEX_3D));

	// �`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);
}

// ��]�p�x�̕␳
float VERTEX_3D::Rotation_Correction(float r)
{
	if (r > 360.0f)
	{
		r -= 360.0f;
	}
	if (r < 0.0f)
	{
		r += 360.0f;
	}
	return r;
}