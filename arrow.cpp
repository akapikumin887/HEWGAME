#include "arrow.h"
#include "player.h"
#include "input.h"

Arrow arrow[ARROW_MAX];

// ARROWの初期化
void Arrow_Initialize()
{
	Create_Arrow();
}

// ARROWの終了処理
void Arrow_Finalize()
{

}

// ARROWの更新
void Arrow_Update()
{
	if (Keyboard_IsPress(DIK_SPACE))
	{
		Create_Arrow();
	}
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (arrow[i].bUse)
		{
			// 発射されていない
			if (!arrow[i].beShotted)
			{
				// 向きベクトルの取得と正規化
				arrow[i].Arrow_Direction_Normalize();
			}
			// 発射されたら
			else
			{
				// プレイヤー情報の取得
				Player *player = GetPlayer();

				// 速度更新
				arrow[i].speed.x = ARROW_SPEED * arrow[i].direction.x * player->charge_span;
				arrow[i].speed.y = ARROW_SPEED * arrow[i].direction.y * player->charge_span;

				// 位置更新
				arrow[i].pos.x += arrow[i].speed.x;
				arrow[i].pos.y += arrow[i].speed.y;

				// 画面外チェック
				if (arrow[i].pos.x >= SCREEN_WIDTH || arrow[i].pos.x <= 0 || arrow[i].pos.y >= SCREEN_HEIGHT || arrow[i].pos.y <= 0)
				{
					arrow[i].bUse = false;
					arrow[i].beShotted = false;
				}
			}
		}
	}
}

// ARROWの描画
void Arrow_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = Mydirect3D_GetDevice();

	for (int i = 0; i < ARROW_MAX; i++)
	{
		//スプライト描画
		if (arrow[i].bUse)//使用中なら処理
		{
			//テクスチャのセット
			pDevice->SetTexture(0, Texture_GetTexture(arrow[i].TextureIndex));

			//ブレンド設定
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
			// サンプラーステートパラメータの設定
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
			pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
			pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

			Sprite_SetColor(arrow[i].color);//色のセット

			//スプライト描画
			Sprite_Draw_Rotation_Arrow(arrow[i].TextureIndex,
				arrow[i].pos.x, arrow[i].pos.y,
				arrow[i].tx, arrow[i].ty,
				arrow[i].tw, arrow[i].th,
				atan2f(arrow[i].direction.y, arrow[i].direction.x));
		}
	}
}

// ARROWの作成
void Create_Arrow()
{
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (!arrow[i].bUse)
		{
			arrow[i].bUse = true;
			arrow[i].pos.x = ARROW_X;
			arrow[i].pos.y = ARROW_Y;
			arrow[i].Arrow_Direction_Normalize();
			break;
		}
	}
}

// ARROWの情報取得
Arrow* GetArrow()
{
	return arrow;
}

Arrow::Arrow()
{
	bUse = false;
	beShotted = false;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_ARROW01;
	tx = Texture_GetWidth(TextureIndex) / 8;
	ty = 0;
	tw = Texture_GetWidth(TextureIndex) / 2;
	th = Texture_GetHeight(TextureIndex);
}

Arrow::~Arrow()
{
	
}

void Arrow::Arrow_Direction_Normalize()
{
	// プレイヤー情報の取得
	Player *player = GetPlayer();
	float len;

	// 矢の発射ベクトル更新
	direction.x = player->pos.x - pos.x;
	direction.y = player->pos.y - pos.y;

	// 向きベクトルの正規化
	len = sqrtf(direction.x * direction.x + direction.y * direction.y);
	direction.x = direction.x / len;
	direction.y = direction.y / len;
}