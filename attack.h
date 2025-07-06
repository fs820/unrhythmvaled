//---------------------------------------
//
//弾表示処理の定義・宣言[attack.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ATTACK_H_
#define _ATTACK_H_

#include"main.h"
#include"player.h"

#define SP_FILE "data\\DATA\\sp%d.txt"
#define MAX_ATTACK (256)//最大数
#define SP_MAX (5)//技数
#define BOM_SIZE_INA (20.0f)//巨大弾サイズ
#define SP_SIZE_INA (5.0f)//スペシャル砲サイズ
#define LONG_ATTACK_SPEED (50.0f)//スピード
#define SP2_ATTACK_SPEED (100.0f)//スピード
#define SP3_ATTACK_SPEED (80.0f)//スピード
#define SP4_ATTACK_SPEED (20.0f)//スピード
#define ATTACK_SPEED_SP (25.0f)//スピード
#define LONG_ATTACK_INTER (20)//発射間隔
#define ALL_ATTACK (10)//全方位弾弾数
#define ALL_ATTACKS (6)//誘導弾弾数
#define ALL_ATTACKSP (200)//スペシャル砲弾数
#define SHORT_ATTACK_LIFE (1)//体力
#define LONG_ATTACK_LIFE (50)//体力
#define SP_ATTACK_LIFE (5)//体力
#define SP2_ATTACK_LIFE (50)//体力
#define SP3_ATTACK_LIFE (100)//体力
#define SP4_ATTACK_LIFE (200)//体力
#define SP5_ATTACK_LIFE (500)//体力
//種類
typedef enum
{
	ATTACKTYPE_PLAYER = 0,//プレイヤー
	ATTACKTYPE_ENEMY,//敵
	ATTACKTYPE_BOSS,//ボス
	ATTACKTYPE_MAX//種類数
}ATTACKTYPE;

//種類
typedef enum
{
	STYLE_SHORT = 0,
	STYLE_SHORT2,
	STYLE_SHORT3,
	STYLE_SHORT4,
	STYLE_SHORT5,
	STYLE_LONG,
	STYLE_LONG2,
	STYLE_LONG3,
	STYLE_LONG4,
	STYLE_LONG5,
	STYLE_SP,
	STYLE_SP2,
	STYLE_SP3,
	STYLE_SP4,
	STYLE_SP5,
	STYLE_MAX//種類数
}ATTACKSTYLE;

void InitAttack(void);//初期化処理
void UpdateAttack(void);//更新処理
void SpAttackMaster(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft, PATA* Pata);//技
void SetShortAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//近接1
void SetShort2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//近接2
void SetShort3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//近接3
void SetShort4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//近接4
void SetShort5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//近接5
void SetLongAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//遠隔1
void SetLong2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//遠隔2
void SetLong3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//遠隔3
void SetLong4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//遠隔4
void SetLong5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//遠隔5
void SetSpAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//技1
void SetSp2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//技2
void SetSp3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//技3
void SetSp4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//技4
void SetSp5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//技5
void SetAllAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, ATTACKTYPE type, int nInter);//全方位弾
void SetSearchAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, ATTACKTYPE type, int nInter);//誘導弾
void SetBomAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, ATTACKTYPE type, int nInter);//巨大弾

#endif _ATTACK_H_