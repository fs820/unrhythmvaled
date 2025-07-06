//------------------------------------------
//
//ゲームメインの処理の定義・宣言[game.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"

#define TIME_LIMIT (90)
#define INTER (60)//終了猶予
#define SCROLL_X (3.0f)
#define SCROLL_Y (-1.0f)

#define EDIT_BLOCK_SPACE (80.0f)

//ゲーム状態の列挙型定義
typedef enum
{
	GAMESTATE_NONE=0,//ゲームでない
	GAMESTATE_NORMAL,//ゲーム中
	GAMESTATE_PAUSE,//ポーズ中
	GAMESTATE_END,//終了
	GAMESTATE_MAX//数
}GAMESTATE;

void InitGame(void);//ゲームの初期化処理
void UninitGame(void);//ゲームの終了処理
void UpdateGame(void);//ゲームの更新処理
void DrawGame(void);//ゲームの描画処理
void SetGameState(GAMESTATE state);//ゲームの状態設定
GAMESTATE GetGameState(void);//ゲーム状態取得
bool GetClear(void);
bool Edit(void);

//デバック用
void EditMode(void);
#endif _GAME_H_
