#include "bg.h"
#include "debug_font.h"

static BG bg01;

// BGの初期化
void BG_Initialize()
{
	
}

// BGの終了処理
void BG_Finalize()
{
	
}

// BGの更新
void BG_Update()
{

}

// BGの描画
void BG_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	// テクスチャのセット
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

	// スプライト描画
	if (bg01.bUse) // 使用中なら処理
	{
		Sprite_SetColor(bg01.color); // 色のセット
		// スプライト描画
		Sprite_Draw(bg01.TextureIndex,
			bg01.pos.x, bg01.pos.y,
			bg01.tx, bg01.ty,
			bg01.tw, bg01.th);
	}
}

BG::BG()
{
	bUse = true; // 構造体使用中
	pos.x = SCREEN_WIDTH / 2;
	pos.y = SCREEN_HEIGHT / 2;
	color = D3DCOLOR_RGBA(200, 200, 200, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_BG01;
	tx = 0;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
}

BG::~BG()
{
	bUse = false;
}