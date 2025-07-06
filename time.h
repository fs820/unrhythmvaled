//---------------------------------------
//
//スコア処理の定義・宣言[time.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _TIME_H_
#define _TIME_H_

#include"main.h"

#define TIME_MAX (2)//桁数
#define U_MAX_S (10)//テクスチャ数(横)
#define V_MAX_S (1)//テクスチャ数(縦)
#define TIME_WIDTH (350.0f)//サイズ
#define TIME_HEIGHT (70.0f)//サイズ
#define DIGIT (10)//進数

//プロトタイプ宣言
void InitTime(void);//スコアの初期化処理
void UninitTime(void);//スコアの終了処理
void UpdateTime(void);//スコアの更新処理
void DrawTime(void);//スコアの描画処理
void SetTime(int nTime, bool bTex);//スコア書き換え
int DigitT(int nData, int nDigit);

#endif _Time_H_
