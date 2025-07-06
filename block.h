//---------------------------------------
//
//弾表示処理の定義・宣言[Block.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include"main.h"

#define MAX_BLOCK (256)//最大数
#define BLOCK_TEX "data\\TEXTURE\\block000.png"
#define BLOCK_WIDTH_DEF (80.0f)
#define BLOCK_HEIGHT_DEF (80.0f)
#define FIELD_HEIGHT (80.0f)
#define BLOCK_INA (8.0f)
#define BLOCK_SPEEDX (3.0f)
#define BLOCK_SPEEDY (-3.0f)
#define CLLISION_INA (0.1f)
#define CLLISION_NUM (10)

//弾構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;//動き
	float fWidth;//大きさ
	float fHeight;//寿命
	bool bDisp;
	bool bUse;//使用しているかどうか
}Block;

void InitBlock(void);//初期化処理
void UninitBlock(void);//終了処理
void UpdateBlock(void);//更新処理
void DrawBlock(void);//描画処理
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);//設定
Block* GetBlock(void);
bool CllisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight, Block** pBlock, bool bPlayer); //当たり判定
void CllisionBlockA(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fLength, bool* pUse); //当たり判定
#endif _BLOCK_H_
