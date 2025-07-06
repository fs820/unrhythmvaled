//---------------------------------------
//
//アイテム表示処理[item.cpp]
//Author fuma sato
//
//---------------------------------------

#include"item.h"
#include"player.h"
#include"score.h"
#include"scroll.h"

//グローバル
LPDIRECT3DTEXTURE9 g_apTextureItem = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;//バッファのポインタ
Item g_aItem[MAX_ITEM];//アイテムの情報
bool g_bGoal = false;

//-----------------
//アイテムの初期化処理
//-----------------
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	//デバイスを取得
	pDevice = GetDevice();

	for (int i = 0; i < MAX_ITEM; i++)
	{
		g_aItem[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[i].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[i].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aItem[i].fRadius = 0.0f;
		g_aItem[i].Type = TYPE_MAX;
		g_aItem[i].bDisp = false;
		g_aItem[i].bUse = false;//使用していない状態にする
	}

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		ITEM_TEX,
		&g_apTextureItem
	);

	g_bGoal = false;

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += VT_MAX;//ポリゴン1つ分進める
	}
	g_pVtxBuffItem->Unlock();//バッファのアンロック
}

//----------------
//アイテムの終了処理
//----------------
void UninitItem(void)
{
	//テクスチャの破棄
	if (g_apTextureItem != NULL)
	{
		g_apTextureItem->Release();
		g_apTextureItem = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//----------------
//アイテムの更新処理
//----------------
void UpdateItem(void)
{
	VERTEX_2D* pVtx;
	static D3DXVECTOR3 Oldpos[MAX_ITEM];

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == true)
		{//アイテムが使用されている
			Oldpos[i] = g_aItem[i].pos;

			Player* pPlayer;
			pPlayer = GetPlayer();
			if (pPlayer->pos.x > g_aItem[i].pos.x - g_aItem[i].fRadius && pPlayer->pos.x< g_aItem[i].pos.x + g_aItem[i].fRadius && pPlayer->pos.y> g_aItem[i].pos.y - g_aItem[i].fRadius && pPlayer->pos.y < g_aItem[i].pos.y + g_aItem[i].fRadius)
			{
				switch (g_aItem[i].Type)
				{
				case TYPE_NORMAL:
					AddScore(SCORE_ITEM);
					break;
				case TYPE_GOAL:
					AddScore(SCORE_GOAL);
					g_bGoal = true;
					break;
				}

				g_aItem[i].bUse = false;
			}

			g_aItem[i].pos.x += g_aItem[i].move.x;
			g_aItem[i].pos.y += g_aItem[i].move.y;

			g_aItem[i].pos -= GetScroll();

			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			//座標設定
			pVtx += i * VT_MAX;//ポリゴン1つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

			g_pVtxBuffItem->Unlock();//バッファのアンロック

			if (g_aItem[i].pos.x <= 0 || g_aItem[i].pos.x >= SCREEN_WIDTH || g_aItem[i].pos.y <= 0 || g_aItem[i].pos.y >= SCREEN_HEIGHT)
			{
				g_aItem[i].bDisp = false;
			}
			else
			{
				g_aItem[i].bDisp = true;
			}
		}
	}
}

//----------------
//アイテムの描画処理
//----------------
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == true&& g_aItem[i].bDisp == true)
		{//アイテムが使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureItem);

			//アイテムの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}
	}
}

//-------------------
//アイテム設置
//-------------------
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, TYPE Type)
{
	VERTEX_2D* pVtx;

	for (int i = 0; i < MAX_ITEM; i++)
	{
		if (g_aItem[i].bUse == false)
		{//アイテムが使用されていない
			g_aItem[i].pos = pos;
			g_aItem[i].move = move;
			g_aItem[i].fRadius = ITEM_SIZE;
			g_aItem[i].Type = Type;

			switch (g_aItem[i].Type)
			{
			case TYPE_NORMAL:
				g_aItem[i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			case TYPE_GOAL:
				g_aItem[i].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				break;
			}

			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標設定
			pVtx += i * VT_MAX;//ポリゴン1つ分進める
			pVtx[0].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y - g_aItem[i].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[i].pos.x - g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[i].pos.x + g_aItem[i].fRadius, g_aItem[i].pos.y + g_aItem[i].fRadius, 0.0f);

			pVtx[0].col = g_aItem[i].col;
			pVtx[1].col = g_aItem[i].col;
			pVtx[2].col = g_aItem[i].col;
			pVtx[3].col = g_aItem[i].col;

			g_pVtxBuffItem->Unlock();//バッファのアンロック

			g_aItem[i].bDisp = true;
			g_aItem[i].bUse = true;

			break;
		}
	}
}

//------------------
//取得
//------------------
Item* GetItem(void)
{
	return &g_aItem[0];
}

//--------------------
//ゴール
//--------------------
bool Goal(void)
{
	return g_bGoal;
}