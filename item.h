//---------------------------------------
//
//アイテム表示処理の定義・宣言[item.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ITEM_H_
#define _ITEM_H_

#include"main.h"

#define ITEM_TEX "data\\TEXTURE\\item.png"
#define MAX_ITEM (128)//最大数
#define ITEM_SIZE (25)
#define ITEM_LIFE (1200)

//アイテムタイプの列挙型定義
typedef enum
{
	TYPE_NORMAL = 0,
	TYPE_GOAL,
	TYPE_MAX
}TYPE;

//アイテム構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	TYPE Type;
	bool bDisp;
	bool bUse;//使用しているかどうか
}Item;

void InitItem(void);//エフェクトの初期化処理
void UninitItem(void);//エフェクトの終了処理
void UpdateItem(void);//エフェクトの更新処理
void DrawItem(void);//エフェクトの描画処理
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move,TYPE Type);//エフェクト設定
Item* GetItem(void);
bool Goal(void);

#endif _ITEM_H_
