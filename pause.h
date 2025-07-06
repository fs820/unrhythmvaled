//------------------------------------------
//
//ポーズ処理の定義・宣言[pause.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _PAUSE_H_
#define _PAUSE_H_

#include"main.h"

#define TEXTURE_RESTART "data\\TEXTURE\\Restart.png"
#define TEXTURE_RETRY "data\\TEXTURE\\Retry.png"
#define TEXTURE_BACKTITLE "data\\TEXTURE\\Back to Title.png"
#define TEXTURE_EXIT "data\\TEXTURE\\Exit.png"

void InitPause(void);//ゲームの初期化処理
void UninitPause(void);//ゲームの終了処理
void UpdatePause(void);//ゲームの更新処理
void DrawPause(void);//ゲームの描画処理

#endif _PAUSE_H_
