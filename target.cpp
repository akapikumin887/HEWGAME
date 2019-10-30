#include "target.h"

static Target target[3];

// Targetの初期化
void Target_Initialize()
{
	target[1].tw = target[2].tw = target[0].tw * 2;
	target[1].pos.x = target[0].pos.x - (target[0].tw + target[1].tw) * 0.5f;
	target[2].pos.x = target[0].pos.x + (target[0].tw + target[2].tw) * 0.5f;
	target[1].color = D3DCOLOR_RGBA(240, 240, 0, 255); // 色を適当に作る
	target[2].color = D3DCOLOR_RGBA(240, 240, 0, 255); // 色を適当に作る
}

// Targetの終了処理
void Target_Finalize()
{

}

// Targetの更新
void Target_Update()
{

}

// Targetの描画
void Target_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	for (int i = 0; i < 3; i++)
	{ // テクスチャのセット
		pDevice->SetTexture(0, Texture_GetTexture(target[i].TextureIndex));
	}

	// ブレンド設定
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	// サンプラーステートパラメータの設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	for (int i = 0; i < 3; i++)
	{
		if (target[i].bUse)
		{
			Sprite_SetColor(target[i].color); // 色のセット
		    // スプライト描画
			Sprite_Draw(target[i].TextureIndex,
				target[i].pos.x, target[i].pos.y,
				target[i].tx, target[i].ty,
				target[i].tw, target[i].th);
		}
	}
}

// Targetの取得
Target* GetTarget()
{
	return target;
}

Target::Target()
{
	bUse = true;
	pos.x = SCREEN_WIDTH * 0.5f;
	pos.y = 10.0f;
	color = D3DCOLOR_RGBA(0, 240, 0, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_MAX;
	tx = 0;
	ty = 0;
	tw = 30;
	th = 20;
}

Target::~Target()
{

}