#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

#include <Windows.h>
#include<math.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"

#include <d3dx9.h>
#include "mode.h"
#include "input.h"
#include "debug_font.h"
#include "camera.h"
#include "fade.h"


//�T���v���|���S���̕\�����̂Ɋւ���p�����[�^


static float g_posx = -5, g_posy =4, g_posz = 0;
static float g_posx2 = 0, g_posy2 =4, g_posz2 = 0;
static float g_posx3 = 5, g_posy3 =4, g_posz3 = 0;
//�g��
static float g_posx4 = -5, g_posy4 = 4, g_posz4 = 0;
static float g_Rotation = 0.0f, g_Rotation2 = 3.0f, g_Rotation3 = 0.01f; // �X�v���C�g��]�p�x 
static float g_velx = 0, g_vely = 0, g_velz = 0;

static Mode mode;
static CameraTP cameraTP;


void Mode::Initialize()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	//�I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 12,  //���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y�^(�o�C�g�P��)

		D3DUSAGE_WRITEONLY, //���_�o�b�t�@�̎g�p�@
		FVF_VERTEX_3D,      //�g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,    //���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuff,     //���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);              //NULL�ɐݒ�

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D  *pVtx;

	//�������ŃC���^�[�t�F�[�X���쐬(�����)
	pDevice->CreateIndexBuffer(sizeof(WORD) * 4,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pD3DIdxBuff,
		NULL);

	WORD* pIdx = NULL; //�z��̐擪�|�C���^�̓��ꕨ


	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	//���_���W�̐ݒ�
	//���
	pVtx[0].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(0.5f, -5.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-2.0f, -5.0f, 0.0f);

	pVtx[3].vtx = D3DXVECTOR3(-2.0f, -5.0f, 0.0f);
	pVtx[4].vtx = D3DXVECTOR3(-2.0f, 0.5f, 0.0f);
	pVtx[5].vtx = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

	
	//�@���x�N�g���̐ݒ�(�����_�ł͂�����Ȃ�)
	//���
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	

	//���ˌ��̐ݒ�(�����_�ł͂�����Ȃ�)
	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

	pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);





	
	//���̏�����
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));
	g_light.Type = D3DLIGHT_DIRECTIONAL; //���C�g���
	//���F
	g_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_light.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//����

	D3DXVECTOR3 vecDin =
		D3DXVECTOR3(0.0f, -0.6f, 1.0f);
	//���̎w�������x�N�g��
	D3DXVec3Normalize(&vecDin, &vecDin);
	g_light.Direction = vecDin; //���K�����ăZ�b�g

	pDevice->SetLight(0, &g_light); //0�Ԃ炢�Ƃփf�[�^���Z�b�g

	pDevice->LightEnable(0, TRUE); //�O�ԃ��C�g��L����


	g_pD3DIdxBuff->Unlock(); //�K���A�����b�N����


	//���_�f�[�^���A�����b�N����
	g_pD3DVtxBuff->Unlock();

	//�����܂Ł@3D�|���S���p���_�̏���
	//================================================================

}

void Mode::Finalize()
{
	//Texture_Release();

	
}

void Mode::Update()
{
	g_posy  += g_vely;
	g_posy2 += g_vely;
	g_posy3 += g_vely;
	g_posy4 += g_vely;
	
	//g_posx4 += g_velx;

	//����
	if (g_posy <= 3.5f )
	{
		
		g_vely += 0.001f;
		
	}
	if (g_posy >= 3.5f )
	{
		
		g_vely += -0.001f;
		
	}
	
	if (g_posx4 <= -5.0)
	{
		g_posx4 = -5.0;
	}
	if (g_posx4 >= 5.0)
	{
		g_posx4 = 5.0;
	}
	//����
	//�E�ړ��ő���W5,���ő���W-5
	
	if (GetKeyboardTrigger(DIK_D))
	{
		g_posx4 += 5.0f;
		
	}
	if (GetKeyboardTrigger(DIK_A))
	{
		g_posx4 -= 5.0f;
	}

	//EASYselect
	if (g_posx4 == -5.0f && GetKeyboardTrigger(DIK_RETURN))
	{
		//scene_easy
		Fade(SCENE_GAME);
	}
	//NORMALselect
	if (g_posx4 == 0.0f && GetKeyboardTrigger(DIK_RETURN))
	{
		//scene_normal
		Fade(SCENE_GAME);
	}
	//HARDselect
	if (g_posx4 == 5.0f && GetKeyboardTrigger(DIK_RETURN))
	{
		//scene_hard
		Fade(SCENE_GAME);
	}


	
}

void Mode::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//������ǉ��@�e�X�g�p�J������������
	
	//=============================================================
	//������ǉ��@�|���S���̃��[���h�s��̍쐬

	//EASY

	D3DXMATRIX mtxScl; //�X�P�[�����O�s��
	D3DXMATRIX mtxRot; //��]�s��
	D3DXMATRIX mtxTrs; //���s�ړ��s��

	g_pos = D3DXVECTOR3(g_posx, g_posy, g_posz); //�ʒu
	g_rot = D3DXVECTOR3(0, g_Rotation,0); //����(��])
	g_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //�傫��(�X�P�[��)

	D3DXMatrixIdentity(&g_mtxworld);     //���[���h�s���P�ʍs��ɏ�����

	//�X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl, g_scl.x, g_scl.y, g_scl.z);
	D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxScl); //World * Scaling

	//���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs, g_pos.x, g_pos.y, g_pos.z);
	D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxTrs); //World * Translation

	//��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rot.y, g_rot.x, g_rot.z);
	D3DXMatrixMultiply(&g_mtxworld, &g_mtxworld, &mtxRot); //World * Rotation


	//���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld);

	//�`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[��(�f�[�^�̒ʂ蓹)�ɃZ�b�g
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//�`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�|���S���̕`��

	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE); //����������ON
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_EASY));
	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


	//��(NORMAL)
	//����
	D3DXMATRIX mtxScl2; //�X�P�[�����O�s��
	D3DXMATRIX mtxRot2; //��]�s��
	D3DXMATRIX mtxTrs2; //���s�ړ��s��

	g_pos2 = D3DXVECTOR3(g_posx2, g_posy2, g_posz2); //�ʒu
	g_rot2 = D3DXVECTOR3(0, g_Rotation, 0); //����(��]
	g_scl2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //�傫��(�X�P�[��)

	D3DXMatrixIdentity(&g_mtxworld2);     //���[���h�s���P�ʍs��ɏ�����

	//�X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl2, g_scl2.x, g_scl2.y, g_scl2.z);
	D3DXMatrixMultiply(&g_mtxworld2, &g_mtxworld2, &mtxScl2); //World * Scaling
    
	//���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs2, g_pos2.x, g_pos2.y, g_pos2.z);
	D3DXMatrixMultiply(&g_mtxworld2, &g_mtxworld2, &mtxTrs2); //World * Translation

	//��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot2, g_rot2.y, g_rot2.x, g_rot2.z);
	D3DXMatrixMultiply(&g_mtxworld2, &g_mtxworld2, &mtxRot2); //World * Rotation


	//���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld2);

	//�`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[��(�f�[�^�̒ʂ蓹)�ɃZ�b�g
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//�`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�|���S���̕`��
	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_NORMAL));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);




	//��(HARD)

	D3DXMATRIX mtxScl3; //�X�P�[�����O�s��
	D3DXMATRIX mtxRot3; //��]�s��
	D3DXMATRIX mtxTrs3; //���s�ړ��s��

	g_pos3 = D3DXVECTOR3(g_posx3, g_posy3, g_posz3); //�ʒu
	g_rot3 = D3DXVECTOR3(0, g_Rotation, 0); //����(��])
	g_scl3 = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //�傫��(�X�P�[��)

	D3DXMatrixIdentity(&g_mtxworld3);     //���[���h�s���P�ʍs��ɏ�����

	//�X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl3, g_scl3.x, g_scl3.y, g_scl3.z);
	D3DXMatrixMultiply(&g_mtxworld3, &g_mtxworld3, &mtxScl3); //World * Scaling

	//���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs3, g_pos3.x, g_pos3.y, g_pos3.z);
	D3DXMatrixMultiply(&g_mtxworld3, &g_mtxworld3, &mtxTrs3); //World * Translation

	//��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot3, g_rot3.y, g_rot3.x, g_rot3.z);
	D3DXMatrixMultiply(&g_mtxworld3, &g_mtxworld3, &mtxRot3); //World * Rotation


	//���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld3);

	//�`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[��(�f�[�^�̒ʂ蓹)�ɃZ�b�g
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//�`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�|���S���̕`��
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_HARD));
	//pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_SELECT));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


	//�g��

	D3DXMATRIX mtxScl4; //�X�P�[�����O�s��
	D3DXMATRIX mtxRot4; //��]�s��
	D3DXMATRIX mtxTrs4; //���s�ړ��s��

	g_pos4 = D3DXVECTOR3(g_posx4, g_posy4, g_posz4); //�ʒu
	g_rot4 = D3DXVECTOR3(0, g_Rotation, 0); //����(��])
	g_scl4 = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //�傫��(�X�P�[��)

	D3DXMatrixIdentity(&g_mtxworld4);     //���[���h�s���P�ʍs��ɏ�����

	//�X�P�[���s����쐬�����[���h�s��֍���
	D3DXMatrixScaling(&mtxScl4, g_scl4.x, g_scl4.y, g_scl4.z);
	D3DXMatrixMultiply(&g_mtxworld4, &g_mtxworld4, &mtxScl4); //World * Scaling

	//���s�ړ��s����쐬�����[���h�s��֍���
	D3DXMatrixTranslation(&mtxTrs4, g_pos4.x, g_pos4.y, g_pos4.z);
	D3DXMatrixMultiply(&g_mtxworld4, &g_mtxworld4, &mtxTrs4); //World * Translation

	//��]�s����쐬�����[���h�s��֍���
	D3DXMatrixRotationYawPitchRoll(&mtxRot4, g_rot4.y, g_rot4.x, g_rot4.z);
	D3DXMatrixMultiply(&g_mtxworld4, &g_mtxworld4, &mtxRot4); //World * Rotation


	//���[���h�}�g���b�N�X��ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxworld4);

	//�`�悵�����|���S���̒��_�o�b�t�@���f�[�^�X�g���[��(�f�[�^�̒ʂ蓹)�ɃZ�b�g
	pDevice->SetStreamSource(0, g_pD3DVtxBuff, 0, sizeof(VERTEX_3D));

	//�`�悵�����|���S���̒��_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�|���S���̕`��
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_MODE_SELECT));

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);








	//DebugFont_Draw(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,"x;%d y;%d",CameraEyeAt_x,CameraEyeAt_y);
}

void Mode_Initialize()
{
	cameraTP.Initialize(D3DXVECTOR3(-1.0f, -2.5f, 40.0f));
	mode.Initialize();
}

void Mode_Finalize()
{
	cameraTP.Finalize(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mode.Finalize();
}

void Mode_Update()
{
	mode.Update();
}

void Mode_Draw()
{
	cameraTP.Draw();
	mode.Draw();
}