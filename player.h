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

class Player
{
public:
	bool bUse;                 // 使用中フラグ
	D3DXVECTOR3 pos;           // 表示座標
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
#endif