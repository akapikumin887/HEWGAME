#include "player.h"
#include "input.h"
#include "debug_font.h"

Player *pplayer;
static DIMOUSESTATE *pMouseState;

// Playerの初期化
void Player_Initialize()
{
	pplayer = new Player;
}

// Playerの終了処理
void Player_Finalize()
{
	delete pplayer;
}

// Playerの更新
void Player_Update()
{
	pMouseState = GetMouseState();

	// Playerの移動
	pplayer->pos.x += pMouseState->lX;
	pplayer->pos.y += pMouseState->lY;

	// 画面外処理
	if ((pplayer->pos.x - pplayer->tw / 2) <= 0.0f)
	{
		pplayer->pos.x = pplayer->tw / 2;
	}
	if ((pplayer->pos.x + pplayer->tw / 2) >= SCREEN_WIDTH)
	{
		pplayer->pos.x = SCREEN_WIDTH - pplayer->tw / 2;
	}
	if ((pplayer->pos.y - pplayer->th / 2) <= 0.0f)
	{
		pplayer->pos.y = pplayer->th / 2;
	}
	if ((pplayer->pos.y + pplayer->th / 2) >= SCREEN_HEIGHT)
	{
		pplayer->pos.y = SCREEN_HEIGHT - pplayer->th / 2;
	}
}

// Playerの描画
void Player_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	//テクスチャのセット
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_PLAYER));

	//ブレンド設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	//スプライト描画
	if (pplayer->bUse)//使用中なら処理
	{
		Sprite_SetColor(pplayer->color);//色のセット
		//スプライト描画
		Sprite_Draw(pplayer->TextureIndex,
			pplayer->pos.x, pplayer->pos.y,
			pplayer->tx, pplayer->ty,
			pplayer->tw, pplayer->th);
		DebugFont_Draw(2, 2, "x: %.2lf y: %.2lf", pplayer->pos.x, pplayer->pos.y);
	}
}

Player::Player()
{
	bUse = true; // 構造体使用中
	pos.x = SCREEN_WIDTH / 2; 
	pos.y = SCREEN_HEIGHT / 2;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_PLAYER;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
	power = 0;
	charge_span = 0;
}

Player::~Player()
{
	bUse = false;
}