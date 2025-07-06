//---------------------------------------
//
//敵表示処理の定義・宣言[enemy.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ENEMY_H_

#define _ENEMY_H_

#include"main.h"
#include"block.h"

//マクロ定義
#define ENEMY_TEX "data\\TEXTURE\\enemy.png"
#define ENEMY_TEX2 "data\\TEXTURE\\enemy2.png"
#define ENEMY_TEX3 "data\\TEXTURE\\enemy3.png"
#define ENEMY_TEX4 "data\\TEXTURE\\enemy4.png"
#define MAX_ENEMY (128)//敵の最大数
#define NUM_ENEMY (4)//敵の種類
#define ENEMY_WIDTH (150)//幅
#define ENEMY_HEIGHT (150)//高さ
#define ENEMY_WIDTH2 (300)//幅
#define ENEMY_HEIGHT2 (300)//高さ
#define ENEMY_WIDTH3 (450)//幅
#define ENEMY_HEIGHT3 (336)//高さ
#define ENEMY_WIDTH4 (560)//幅
#define ENEMY_HEIGHT4 (600)//高さ
#define ENEMY_LIFE (9)//体力
#define ENEMY_LIFE2 (99)//体力
#define ENEMY_LIFE3 (999)//体力
#define ENEMY_LIFE4 (9999)//体力

#define ENEMY_SPEED_DEF (0.0f)//基本スピード
#define ENEMY_MAX_SPEED (50.0f)//速さ
#define ENEMY_JUMP (-30.0f)//ジャンプ力
#define ENEMY_JUMP_NUM (1)//ジャンプ回数
#define ENEMY_JUMP_MOVE_INA (0.5f)//ジャンプ移動制御
#define ENEMY_INA (0.3f)//慣性

#define HIT_VECTOR (50.0f)

//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,//通常
	ENEMYSTATE_DAMAGE,//ダメージ
	ENEMYSTATE_MAX,//数
}ENEMYSTATE;

//敵構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 Oldpos;//前フレームのpos
	D3DXVECTOR3 move;//プレイヤースピード管理
	int nType;//種類
	ENEMYSTATE state;//状態
	int nCounterState;//状態カウンター
	float fLength;//大きさ
	float fLengthDef;//対角線の長さ(大きさ)の初期値
	int nLife;//体力
	bool bMove;//動いているか
	bool bJump;//ジャンプしているか
	int JumpCount;
	bool bLeft;
	bool bDisp;
	bool bUse;//つかっているか
	Block* pBlock;
}Enemy;

void InitEnemy(void);//敵の初期化処理
void UninitEnemy(void);//敵の終了処理
void UpdateEnemy(void);//敵の更新処理
void DrawEnemy(void);//敵の描画処理
void SetEnemy(D3DXVECTOR3 pos, int nType);//敵設定
void HitEnemy(int nCntEnemy, int nDamage, D3DXVECTOR3 pos);//敵ヒット
Enemy* GetEnemy(void);//敵情報取得
int GetNumEnemy(void);//敵の数取得

void UpdateEditEnemy(void);

#endif _ENEMY_H_

