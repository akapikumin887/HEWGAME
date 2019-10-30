#include "arrow.h"
#include "input.h"
#include "debug_font.h"
<<<<<<< HEAD
#include "player.h"
#include "target.h"
#include "score.h"

static Arrow arrow[ARROW_MAX];
int Arrow::cnt = 0;
static Target *target = GetTarget();
=======

Arrow arrow[ARROW_MAX];
int Arrow::cnt = 0;
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83

// ARROWの初期化
void Arrow_Initialize()
{

}

// ARROWの終了処理
void Arrow_Finalize()
{

}

// ARROWの更新
void Arrow_Update()
{
<<<<<<< HEAD
	// シーン遷移
	if (Arrow::cnt == 5)
	{
		SetScene(SCENE_RESULT);
	}
	
=======
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
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
<<<<<<< HEAD
			{   
				// 命中していない
				if (!arrow[i].hit)
=======
			{
				// 速度更新
				arrow[i].move.x = ARROW_SPEED * arrow[i].direction.x * arrow[i].charge;
				arrow[i].move.y = ARROW_SPEED * arrow[i].direction.y * arrow[i].charge;

				// 位置更新
				arrow[i].pos.x += arrow[i].move.x;
				arrow[i].pos.y += arrow[i].move.y;

				// 画面外チェック
				if (arrow[i].pos.x >= SCREEN_WIDTH || arrow[i].pos.x <= 0 || arrow[i].pos.y >= SCREEN_HEIGHT || arrow[i].pos.y <= 0)
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
				{
					// 速度更新
					arrow[i].move.x = ARROW_SPEED * arrow[i].direction.x * arrow[i].charge;
					arrow[i].move.y = ARROW_SPEED * arrow[i].direction.y * arrow[i].charge;

					// 矢の尾の位置更新
					arrow[i].posTail.x += arrow[i].move.x;
					arrow[i].posTail.y += arrow[i].move.y;

					// 矢の先頭の位置更新
					arrow[i].Arrow_Head_Pos();

					// 画面外チェック
					if (arrow[i].posTail.x >= SCREEN_WIDTH || arrow[i].posTail.x <= 0 || arrow[i].posTail.y >= SCREEN_HEIGHT || arrow[i].posTail.y <= 0)
					{
						arrow[i].bUse = false;
						arrow[i].beShotted = false;
					}

					// 当たり判定
					for (int j = 0; j < 3; j++)
					{
						// 縦判定
						if (arrow[i].posHead.y <= target[j].pos.y + target[j].th / 2)
						{
							// 横判定
							if (arrow[i].posHead.x >= target[j].pos.x - target[j].tw / 2 && arrow[i].posHead.x <= target[j].pos.x + target[j].tw / 2)
							{
								// 命中
								arrow[i].hit = true;
								if (j == 0)
								{
									Add_Score(5);
								}
								else
								{
									Add_Score(1);
								}
							}
						}
					}
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
		// スプライト描画
		if (arrow[i].bUse) // 使用中なら処理
		{
			// テクスチャのセット
			pDevice->SetTexture(0, Texture_GetTexture(arrow[i].TextureIndex));

			// ブレンド設定
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			// αブレンドを行う
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
			// サンプラーステートパラメータの設定
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(U値)を設定
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);// テクスチャアドレッシング方法(V値)を設定
			pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
			pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

			Sprite_SetColor(arrow[i].color); // 色のセット

			// スプライト描画
			Sprite_Draw_Rotation_Arrow(arrow[i].TextureIndex,
				arrow[i].posTail.x, arrow[i].posTail.y,
				arrow[i].tx, arrow[i].ty,
				arrow[i].tw, arrow[i].th,
				arrow[i].degree.z);
		}
	}
	Arrow::Print();
}

// ARROWの作成
void Create_Arrow()
{
	for (int i = 0; i < ARROW_MAX; i++)
	{
		if (!arrow[i].bUse && Arrow::cnt < ARROW_MAX)
		{
			arrow[i].bUse = true;
			arrow[i].posTail.x = ARROW_X;
			arrow[i].posTail.y = ARROW_Y;
			arrow[i].Arrow_Direction_Normalize();
			Arrow::cnt++;
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
	hit = false;
	color = D3DCOLOR_RGBA(255, 255, 255, 255); // 色を適当に作る
	TextureIndex = TEXTURE_INDEX_ARROW01;
	tx = Texture_GetWidth(TextureIndex);
	ty = 0;
	tw = Texture_GetWidth(TextureIndex);
	th = Texture_GetHeight(TextureIndex);
	charge = 0;
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
	direction.x = player->pos.x - posTail.x;
	direction.y = player->pos.y - posTail.y;

	// 向きベクトルの正規化
	len = sqrtf(direction.x * direction.x + direction.y * direction.y);
	direction.x = direction.x / len;
	direction.y = direction.y / len;
	degree.z = atan2f(direction.y, direction.x);
}

<<<<<<< HEAD
void Arrow::Arrow_Head_Pos()
{
	posHead.x = posTail.x + Texture_GetWidth(TextureIndex) * direction.x;
	posHead.y = posTail.y + Texture_GetWidth(TextureIndex) * direction.y;
}

=======
>>>>>>> 38e7617f6fb0a07da2bbb813709ac1b856619b83
void Arrow::Print()
{
	DebugFont_Draw(2, 32, "残りの本数: %d", ARROW_MAX - Arrow::cnt);
}