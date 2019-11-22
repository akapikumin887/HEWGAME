#include "score.h"
#include "debug_font.h"
#include"input.h"
#include"sprite.h"
#include"texture.h"

int score;
int g_Score;
static Score score1;
// Scoreの初期化
void Score_Initialize()
{
	score = 0000;
}

// Scoreの終了処理
void Score_Finalize()
{
	score = 0000;
}

// Scoreの更新
void Score_Update()
{
	
}

// Scoreの描画
void Score_Draw()
{
	
	

	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// テクスチャのセット
	pDevice->SetTexture(0, Texture_GetTexture(score1.TextureIndex));
	// ブレンド設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

		Sprite_SetColor(score1.color); // 色のセット
		// スプライト

		g_Score = score;
		g_Score = g_Score % 10;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64*4, score1.pos.y = 24,
			score1.tx = g_Score*64, score1.ty = 0,
			64, 64);
		g_Score = score / 10;
		g_Score = g_Score % 10;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64*3, score1.pos.y = 24,
			score1.tx = g_Score * 64, score1.ty = 0,
			64, 64);
		g_Score = score / 100;
		g_Score = g_Score % 10;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64*2, score1.pos.y = 24,
			score1.tx = g_Score * 64, score1.ty = 0,
			64, 64);
		g_Score = score / 1000;
		Sprite_Draw(score1.TextureIndex,
			score1.pos.x = 64, score1.pos.y = 24,
			score1.tx = g_Score * 64, score1.ty = 0,
			64, 64);






}

// Scoreの加算
void Add_Score(int s)
{
	score += s;
}





// スコアの取得
int* GetScore()
{
	return &score;
}