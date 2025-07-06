//---------------------------------------
//
//パーティクル表示処理の定義・宣言[exef.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"

#define MAX_PARTICLE (128)//最大数
#define PARTICLE_NUM (5)//フレーム発生数
#define PARTICLE_LIFE (80)//残留具合

void InitParticle(void);//パーティクルの初期化処理
void UninitParticle(void);//パーティクルの終了処理
void UpdateParticle(void);//パーティクルの更新処理
void SetParticle(D3DXVECTOR3 pos,float fLength);//パーティクル設定

#endif _PARTICLE_H_