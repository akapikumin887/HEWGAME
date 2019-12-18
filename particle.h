/*

	���m�[�T�|�[�g�ł���Ώ���ɎQ�l�����p���Ă��n�j

	�������N���X�ɂȂ��Ă���̂ŁAC���ꕗ�ɂ���Ȃ�
	���̂܂܊��p����Ȃ�͎��R
	�Œ���̓��e�Ŕėp���͂��܂�Ȃ�

*/

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cos���g���̂ɕK�v
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

///////////////////////////////////////////////////////////
//
//particle_base
//	�p�[�e�B�N�����̂��̂�\���N���X
//
///////////////////////////////////////////////////////////

//=================================
//�p�[�e�B�N���i���q�j�̊�{����
//==================================
class particle_base
{
	private: // �v���C�x�[�g�����o�[�ϐ�
		D3DXVECTOR3	pos;       // ���W
		D3DXVECTOR3	scl;	   // �X�P�[��
		D3DXVECTOR4	color;	   // �F
		D3DXVECTOR3 direction; // ����
		D3DXVECTOR3 move;      // �ړ�
		int	Life;	           // ����	
		
	public: //
		particle_base()	// �R���X�g���N�^
		{
			Life = -1;	// Life = -1���������v��
		}

	public://���J�����o�[�֐�
		int		GetLife(){return Life;}; // �����Q�b�^�[
		void	InitParticle();	// ������
		void	UpdateParticle(); // �X�V
		void	DrawParticle();	// �\��
};

///////////////////////////////////////////////////////////
//
//PARTICLE
//	�p�[�e�B�N�����Ǘ�����N���X
//  main�֐�������Ăяo��
//
///////////////////////////////////////////////////////////

//=====================================
//�p�[�e�B�N���Ǘ�
//=====================================
class PARTICLE
{
	private: // �v���C�x�[�g�����o�[�ϐ�
		static	particle_base  *obj_pool; // �p�[�e�B�N���̍\���̔z��
		static  int numparticle;		  // �p�[�e�B�N���\���̔z��̐�
		static  VERTEX_3D  vertex[4];	  // �p�[�e�B�N���̒��_�f�[�^

	private://�N���X���Ŏg�p���郁���o�[�֐�
		static	void ParticleOccer(); // �p�[�e�B�N���𔭐�����������֐�

	public:	// main�����Ăяo�����J�����o�[�֐�
		static	void Init(int num_particle); // �������i�z��̐����w��j
		static	void Update();				 // �S�Ẵp�[�e�B�N�����X�V
		static	void Draw();				 // �S�Ẵp�[�e�B�N����\��
		static	void Uninit();				 // ��n��

	public:	// �K�v�ɉ����ăp�[�e�B�N�������Ăяo�����J�����o�[�֐�
		static  VERTEX_3D* GetVertex(){ return vertex; }; // ���_�z��̃|�C���^�Q�b�^�[
		static	void SetColor(D3DXVECTOR4 col);	// �p�[�e�B�N���̐F��ς���
//		static	void SetUV(D3DXVECTOR2 uv);
};
#endif