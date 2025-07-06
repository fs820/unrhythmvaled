//---------------------------------------
//
//�G�t�F�N�g�\�������̒�`�E�錾[effect.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include"main.h"

#define MAX_EFFECT (4096)//�ő吔
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


//���
typedef enum
{
	EFFECTTYPE_NORMAL = 0,//�v���C���[1
	EFFECTTYPE_SHORT,//�ߐ�
	EFFECTTYPE_SHORT2,//�ߐ�
	EFFECTTYPE_SHORT3,//�ߐ�
	EFFECTTYPE_SHORT4,//�ߐ�
	EFFECTTYPE_SHORT5,//�ߐ�
	EFFECTTYPE_LONG,//���u
	EFFECTTYPE_LONG2,//���u
	EFFECTTYPE_LONG3,//���u
	EFFECTTYPE_LONG4,//���u
	EFFECTTYPE_LONG5,//���u
	EFFECTTYPE_SP,//�Z
	EFFECTTYPE_SP2,//�Z
	EFFECTTYPE_SP3,//�Z
	EFFECTTYPE_SP4,//�Z
	EFFECTTYPE_SP5,//�Z
	EFFECTTYPE_MAX//��ސ�
}EFFECTTYPE;

void InitEffect(void);//�G�t�F�N�g�̏���������
void UninitEffect(void);//�G�t�F�N�g�̏I������
void UpdateEffect(void);//�G�t�F�N�g�̍X�V����
void DrawEffect(void);//�G�t�F�N�g�̕`�揈��
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, EFFECTTYPE Type, bool bLeft);//�G�t�F�N�g�ݒ�

#endif _EFFECT_H_