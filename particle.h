/*

	※ノーサポートであれば勝手に参考＆流用してもＯＫ

	ただしクラスになっているので、C言語風にするなり
	そのまま活用するなりは自由
	最低限の内容で汎用性はあまりない

*/

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <stdio.h>
#include <math.h>	//sin cosを使うのに必要
#include "common.h"
#include "mydirect3d.h"
#include "sprite.h"
#include "texture.h"

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

///////////////////////////////////////////////////////////
//
//particle_base
//	パーティクルそのものを表すクラス
//
///////////////////////////////////////////////////////////

//=================================
//パーティクル（粒子）の基本部分
//==================================
class particle_base
{
	private: // プライベートメンバー変数
		D3DXVECTOR3	pos;       // 座標
		D3DXVECTOR3	scl;	   // スケール
		D3DXVECTOR4	color;	   // 色
		D3DXVECTOR3 direction; // 方向
		D3DXVECTOR3 move;      // 移動
		int	Life;	           // 寿命	
		
	public: //
		particle_base()	// コンストラクタ
		{
			Life = -1;	// Life = -1が初期化要求
		}

	public://公開メンバー関数
		int		GetLife(){return Life;}; // 寿命ゲッター
		void	InitParticle();	// 初期化
		void	UpdateParticle(); // 更新
		void	DrawParticle();	// 表示
};

///////////////////////////////////////////////////////////
//
//PARTICLE
//	パーティクルを管理するクラス
//  main関数側から呼び出す
//
///////////////////////////////////////////////////////////

//=====================================
//パーティクル管理
//=====================================
class PARTICLE
{
	private: // プライベートメンバー変数
		static	particle_base  *obj_pool; // パーティクルの構造体配列
		static  int numparticle;		  // パーティクル構造体配列の数
		static  VERTEX_3D  vertex[4];	  // パーティクルの頂点データ

	private://クラス内で使用するメンバー関数
		static	void ParticleOccer(); // パーティクルを発生させる内部関数

	public:	// main側が呼び出す公開メンバー関数
		static	void Init(int num_particle); // 初期化（配列の数を指定）
		static	void Update();				 // 全てのパーティクルを更新
		static	void Draw();				 // 全てのパーティクルを表示
		static	void Uninit();				 // 後始末

	public:	// 必要に応じてパーティクル側が呼び出す公開メンバー関数
		static  VERTEX_3D* GetVertex(){ return vertex; }; // 頂点配列のポインタゲッター
		static	void SetColor(D3DXVECTOR4 col);	// パーティクルの色を変える
//		static	void SetUV(D3DXVECTOR2 uv);
};
#endif