//---------------------------------------
//
//スコア処理[time.cpp]
//Author fuma sato
//
//---------------------------------------

#include"Time.h"

LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;//バッファのポインタ
D3DXVECTOR3 g_posTime;//スコアの位置
int g_nTime;//スコアの値
//-------------------------
//ポリゴンの初期化処理
//-------------------------
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ
	VERTEX_2D* pVtx;//頂点情報ポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//バッファーの設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * VT_MAX * TIME_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_NUMBER,
		&g_pTextureTime
	);

	g_posTime = D3DXVECTOR3((TIME_WIDTH / U_MAX_S) / 2, (TIME_HEIGHT / V_MAX_S) / 2, 0.0f);
	g_nTime = 0;

	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

	for (int i = 0; i < TIME_MAX; i++)
	{
		//座標設定
		pVtx[0].pos = D3DXVECTOR3(g_posTime.x - (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y - (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);
		pVtx[1].pos = D3DXVECTOR3(g_posTime.x + (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y - (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);
		pVtx[2].pos = D3DXVECTOR3(g_posTime.x - (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y + (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);
		pVtx[3].pos = D3DXVECTOR3(g_posTime.x + (TIME_WIDTH / U_MAX_S) / 2, g_posTime.y + (TIME_HEIGHT / V_MAX_S) / 2, g_posTime.z);

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
		pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, UV_DEF / V_MAX_S);
		pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S, UV_DEF / V_MAX_S);

		g_posTime.x += TIME_WIDTH / U_MAX_S;
		pVtx += VT_MAX;
	}

	g_pVtxBuffTime->Unlock();//プレイヤーバッファのアンロック
}

//--------------------------
//ポリゴンの終了処理
//--------------------------
void UninitTime(void)
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//--------------------------
//ポリゴンの更新処理
//--------------------------
void UpdateTime(void)
{

}

//----------------------------
//ポリゴンの描画処理
//----------------------------
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファ
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < TIME_MAX; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTime);

		//背景の描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,//タイプ
			VT_MAX * i,//始まりの番号
			2//ポリゴンの個数
		);
	}
}

//-----------------------------
//スコアの書き換え(保存)
//-----------------------------
void SetTime(int nTime, bool bTex)
{
	VERTEX_2D* pVtx;//頂点情報ポインタ
	int aPosTexU[TIME_MAX];
	g_nTime = nTime;

	//スコア設定
	for (int i = 0; i < TIME_MAX; i++)
	{
		aPosTexU[i] = DigitT(g_nTime, i);
	}

	if (bTex)
	{
		//座標
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//プレイヤーバッファのロック

		for (int i = 0; i < TIME_MAX; i++)
		{
			//テクスチャ
			pVtx[0].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[1].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S));
			pVtx[2].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i], UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);
			pVtx[3].tex = D3DXVECTOR2(UV_DEF / U_MAX_S * aPosTexU[i] + UV_DEF / U_MAX_S, UV_DEF / V_MAX_S * (aPosTexU[i] / U_MAX_S) + UV_DEF / V_MAX_S);

			pVtx += VT_MAX;
		}

		g_pVtxBuffTime->Unlock();//プレイヤーバッファのアンロック
	}
}

//--------------------
//桁分割処理
//--------------------
int DigitT(int nData, int nDigit)
{
	int aNumber[TIME_MAX] = { 0 };//分割用配列
	int nData2 = 1;//計算用

	for (int i = TIME_MAX - 1; i >= 0; i--)
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