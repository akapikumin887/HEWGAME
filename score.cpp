#include "score.h"
#include "debug_font.h"
#include"input.h"

static Number score;
static Score alphabet;

// Scoreの初期化
void Score_Initialize()
{	
	// Alphabetの初期化
	strncpy(alphabet.sa, "SCORE", 8);
	alphabet.len = strlen(alphabet.sa);
}

// Scoreの終了処理
void Score_Finalize()
{
	score.n = 0;
}

// Scoreの更新
void Score_Update()
{
	
}

// Scoreの描画
void Score_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	// テクスチャのセット
	pDevice->SetTexture(0, Texture_GetTexture(score.TextureIndex));
	// ブレンド設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	Sprite_SetColor_2D(score.color); // 色のセット
	
	// スプライト

	for (int i = 0; i < SCORE_DIGIT_MAX; i++)
	{
		score.n_tmp = score.n / (int)powf((float)10, (float)i);
		score.n_tmp = score.n_tmp % 10;
		Sprite_Draw_2D(score.TextureIndex,
			SCREEN_WIDTH - score.pos.x * i * 2 - score.pos.x, score.pos.y,
			score.tx + score.n_tmp * score.tw, score.ty,
			score.tw, score.th);
	}
	
	for (int i = 0; i < alphabet.len; i++)
	{
		Sprite_Draw_2D(alphabet.TextureIndex,
			SCREEN_WIDTH - score.pos.x * DIGIT_MAX * 2 - alphabet.pos.x * (alphabet.len - i) * 2, alphabet.pos.y,
			alphabet.tx + ((alphabet.sa[i] - 65) % 13) * alphabet.tw, alphabet.ty + (alphabet.sa[i] - 65) / 13 * alphabet.th,
			alphabet.tw, alphabet.th);
	}
}

// Scoreの加算
void Add_Score(int s)
{
	score.n += s;
}

// スコアの取得
Number* Get_Score()
{
	return &score;
}