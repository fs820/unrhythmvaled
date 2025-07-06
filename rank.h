//------------------------------------------
//
//ランキングの処理の定義・宣言[rank.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _RANK_H_
#define _RANK_H_

#include"main.h"

#define TEXTURE_BACKTITLE "data\\TEXTURE\\Back to Title.png"
#define TEXTURE_RETRY "data\\TEXTURE\\Retry.png"
#define TEXTURE_EXIT "data\\TEXTURE\\Exit.png"
#define TEXTURE_RANK "data\\TEXTURE\\Number.png"

void InitRank(void);//初期化処理
void UninitRank(void);//終了処理
void UpdateRank(void);//更新処理
void DrawRank(void);//描画処理

#endif _RANK_H_