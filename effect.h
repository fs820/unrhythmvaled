//---------------------------------------
//
//エフェクト表示処理の定義・宣言[effect.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define MAX_EFFECT (4096)//最大数
#define EFFECT_INA (5.0f)
#define TEXTURE_EFFECT "data\\TEXTURE\\effect000.jpg"
#define TEXTURE_EFFECT_SHORT "data\\TEXTURE\\effect001.png"
#define TEXTURE_EFFECT_SHORT2 "data\\TEXTURE\\effect002.png"
#define TEXTURE_EFFECT_SHORT3 "data\\TEXTURE\\effect003.png"
#define TEXTURE_EFFECT_SHORT4 "data\\TEXTURE\\effect004.jpg"
#define TEXTURE_EFFECT_SHORT5 "data\\TEXTURE\\effect005.jpg"
#define TEXTURE_EFFECT_LONG "data\\TEXTURE\\effect006.png"
#define TEXTURE_EFFECT_LONG2 "data\\TEXTURE\\effect007.png"
#define TEXTURE_EFFECT_LONG3 "data\\TEXTURE\\effect008.png"
#define TEXTURE_EFFECT_LONG4 "data\\TEXTURE\\effect009.png"
#define TEXTURE_EFFECT_LONG5 "data\\TEXTURE\\effect010.png"
#define TEXTURE_EFFECT_SP "data\\TEXTURE\\effect011.png"
#define TEXTURE_EFFECT_SP2 "data\\TEXTURE\\effect012.png"
#define TEXTURE_EFFECT_SP3 "data\\TEXTURE\\effect013.png"
#define TEXTURE_EFFECT_SP4 "data\\TEXTURE\\effect014.png"
#define TEXTURE_EFFECT_SP5 "data\\TEXTURE\\effect015.png"


//種類
typedef enum
{
	EFFECTTYPE_NORMAL = 0,//プレイヤー1
	EFFECTTYPE_SHORT,//近接
	EFFECTTYPE_SHORT2,//近接
	EFFECTTYPE_SHORT3,//近接
	EFFECTTYPE_SHORT4,//近接
	EFFECTTYPE_SHORT5,//近接
	EFFECTTYPE_LONG,//遠隔
	EFFECTTYPE_LONG2,//遠隔
	EFFECTTYPE_LONG3,//遠隔
	EFFECTTYPE_LONG4,//遠隔
	EFFECTTYPE_LONG5,//遠隔
	EFFECTTYPE_SP,//技
	EFFECTTYPE_SP2,//技
	EFFECTTYPE_SP3,//技
	EFFECTTYPE_SP4,//技
	EFFECTTYPE_SP5,//技
	EFFECTTYPE_MAX//種類数
}EFFECTTYPE;

void InitEffect(void);//エフェクトの初期化処理
void UninitEffect(void);//エフェクトの終了処理
void UpdateEffect(void);//エフェクトの更新処理
void DrawEffect(void);//エフェクトの描画処理
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE Type, bool bLeft);//エフェクト設定

#endif _EFFECT_H_