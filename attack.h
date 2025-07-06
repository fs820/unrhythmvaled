//---------------------------------------
//
//�e�\�������̒�`�E�錾[attack.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ATTACK_H_
#define _ATTACK_H_

#include"main.h"
#include"player.h"

#define SP_FILE "data\\DATA\\sp%d.txt"
#define MAX_ATTACK (256)//�ő吔
#define SP_MAX (5)//�Z��
#define BOM_SIZE_INA (20.0f)//����e�T�C�Y
#define SP_SIZE_INA (5.0f)//�X�y�V�����C�T�C�Y
#define LONG_ATTACK_SPEED (50.0f)//�X�s�[�h
#define SP2_ATTACK_SPEED (100.0f)//�X�s�[�h
#define SP3_ATTACK_SPEED (80.0f)//�X�s�[�h
#define SP4_ATTACK_SPEED (20.0f)//�X�s�[�h
#define ATTACK_SPEED_SP (25.0f)//�X�s�[�h
#define LONG_ATTACK_INTER (20)//���ˊԊu
#define ALL_ATTACK (10)//�S���ʒe�e��
#define ALL_ATTACKS (6)//�U���e�e��
#define ALL_ATTACKSP (200)//�X�y�V�����C�e��
#define SHORT_ATTACK_LIFE (1)//�̗�
#define LONG_ATTACK_LIFE (50)//�̗�
#define SP_ATTACK_LIFE (5)//�̗�
#define SP2_ATTACK_LIFE (50)//�̗�
#define SP3_ATTACK_LIFE (100)//�̗�
#define SP4_ATTACK_LIFE (200)//�̗�
#define SP5_ATTACK_LIFE (500)//�̗�
//���
typedef enum
{
	ATTACKTYPE_PLAYER = 0,//�v���C���[
	ATTACKTYPE_ENEMY,//�G
	ATTACKTYPE_BOSS,//�{�X
	ATTACKTYPE_MAX//��ސ�
}ATTACKTYPE;

//���
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
	STYLE_MAX//��ސ�
}ATTACKSTYLE;

void InitAttack(void);//����������
void UpdateAttack(void);//�X�V����
void SpAttackMaster(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft, PATA* Pata);//�Z
void SetShortAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�ߐ�1
void SetShort2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�ߐ�2
void SetShort3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�ߐ�3
void SetShort4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�ߐ�4
void SetShort5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�ߐ�5
void SetLongAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//���u1
void SetLong2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//���u2
void SetLong3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//���u3
void SetLong4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//���u4
void SetLong5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//���u5
void SetSpAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�Z1
void SetSp2Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�Z2
void SetSp3Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�Z3
void SetSp4Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�Z4
void SetSp5Attack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, ATTACKTYPE type, bool bLeft);//�Z5
void SetAllAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, ATTACKTYPE type, int nInter);//�S���ʒe
void SetSearchAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, ATTACKTYPE type, int nInter);//�U���e
void SetBomAttack(D3DXVECTOR3 pos, float Playerrot, float fLengthPlayer, int nLife, ATTACKTYPE type, int nInter);//����e

#endif _ATTACK_H_