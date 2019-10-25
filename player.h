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

#ifndef PLAYER_H_
#define PLAYER_H_

#define CHARGE_SPAN 50

class Player
{
public:
	bool bUse;                 // 使用中フラグ
	bool isShot;               // 発射フラグ
	D3DXVECTOR3 pos;           // 発射始点位置情報
	D3DXVECTOR3 degree;        // 発射角度
	D3DCOLOR color;	           // カラー
	TextureIndex TextureIndex; // テクスチャID
	int	tx, ty;	               // テクスチャ貼り付け左上座標
	int	tw, th;	               // テクスチャ貼り付けサイズ
	int power;                 // 弓を引く力
	int	charge_span;           // チャージ間隔
	

	Player();
	~Player();
};

void Player_Initialize(); // Playerの初期化
void Player_Finalize(); // Playerの終了処理
void Player_Update(); // Playerの更新
void Player_Draw(); // Playerの描画
Player* GetPlayer(); // プレイヤー情報の取得
#endif