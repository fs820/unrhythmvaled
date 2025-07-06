//---------------------------------------
//
//背景表示処理[BlockNumber.cpp]
//Author fuma sato
//
//---------------------------------------

#include"blocknumber.h"
#include"block.h"
#include"time.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureBlockNumber = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlockNumber = NULL;//バッファのポインタ
D3DXVECTOR3 g_posNumber;

//----------------------
//背景の初期化処理
//----------------------
void InitBlockNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファの作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * MAX_BLOCK * NUM_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlockNumber,
		NULL
	);

	//1つ目

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_pTextureBlockNumber
	);

	//初期化
	g_posNumber = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_pVtxBuffBlockNumber->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	for (int i = 0; i < MAX_BLOCK * NUM_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_posNumber.x - (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
		pVtx[1].pos = D3DXVECTOR3(g_posNumber.x + (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
		pVtx[2].pos = D3DXVECTOR3(g_posNumber.x - (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
		pVtx[3].pos = D3DXVECTOR3(g_posNumber.x + (((TIME_WIDTH * SIZE_INA) / 10.0f) / U_MAX_S) / 2, g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);

		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//テクスチャ
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, UV_DEF / V_MAX_S);

		pVtx += VT_MAX;
	}

	g_pVtxBuffBlockNumber->Unlock();//バッファのアンロック
}

//-------------------
//背景終了処理
//-------------------
void UninitBlockNumber(void)
{
	//テクスチャの破棄
	if (g_pTextureBlockNumber != NULL)
	{
		g_pTextureBlockNumber->Release();
		g_pTextureBlockNumber = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBlockNumber != NULL)
	{
		g_pVtxBuffBlockNumber->Release();
		g_pVtxBuffBlockNumber = NULL;
	}
}

//-------------------
//背景更新処理
//-------------------
void UpdateBlockNumber(void)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	Block* pBlock;
	pBlock = GetBlock();
	float TexU = 0.0f;

	g_pVtxBuffBlockNumber->Lock(0, 0, (void**)&pVtx, 0);//バッファのロック

	for (int i = 0; i < MAX_BLOCK; i++, pBlock++)
	{
		if (pBlock->bUse)
		{
			g_posNumber = pBlock->pos;
			for (int i2 = 0; i2 < NUM_MAX; i2++)
			{
				TexU = (float)DigitBN(i, i2);
				//座標設定
				pVtx[0].pos = D3DXVECTOR3(g_posNumber.x - ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
				pVtx[1].pos = D3DXVECTOR3(g_posNumber.x + ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y - ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
				pVtx[2].pos = D3DXVECTOR3(g_posNumber.x - ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);
				pVtx[3].pos = D3DXVECTOR3(g_posNumber.x + ((TIME_WIDTH * SIZE_INA) / U_MAX_S) / 2 + (i2 - 1) * ((TIME_WIDTH * SIZE_INA) / U_MAX_S), g_posNumber.y + ((TIME_HEIGHT * SIZE_INA) / V_MAX_S) / 2, g_posNumber.z);

				//テクスチャ
				pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU, UV_DEF / V_MAX_S);
				pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S);
				pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU, UV_DEF / V_MAX_S + UV_DEF / V_MAX_S);
				pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * TexU + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S + UV_DEF / V_MAX_S);

				pVtx += VT_MAX;
			}
		}
		else
		{
			pVtx += VT_MAX * NUM_MAX;
		}
	}

	g_pVtxBuffBlockNumber->Unlock();//バッファのアンロック
}

//-------------------
//背景描画処理
//-------------------
void DrawBlockNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	Block* pBlock;

	pBlock = GetBlock();
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffBlockNumber, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBlockNumber);

	for (int i = 0; i < MAX_BLOCK * NUM_MAX; i++)
	{
		if (pBlock->bUse)
		{
			//背景の描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,//タイプ
				i * VT_MAX,//始まりの番号
				2//ポリゴンの個数
			);
		}

		if (i % NUM_MAX == 0)
		{
			pBlock++;
		}
	}
}

//--------------------
//桁分割処理
//--------------------
int DigitBN(int nData, int nDigit)
{
	int aNumber[NUM_MAX] = { 0 };//分割用配列
	int nData2 = 1;//計算用

	for (int i = NUM_MAX - 1; i >= 0; i--)
	{//桁数分
		if (i == 0)
		{
			aNumber[i] = nData / nData2;//最上位の桁
		}
		aNumber[i] = nData % (nData2 * DIGIT) / nData2;//計算
		nData2 *= DIGIT;//桁引き上げ
	}

	return aNumber[nDigit];//指定の桁を返す
}