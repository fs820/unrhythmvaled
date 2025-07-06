//---------------------------------------
//
//�G�\�������̒�`�E�錾[enemy.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ENEMY_H_

#define _ENEMY_H_

#include"main.h"
#include"block.h"

//�}�N����`
#define ENEMY_TEX "data\\TEXTURE\\enemy.png"
#define ENEMY_TEX2 "data\\TEXTURE\\enemy2.png"
#define ENEMY_TEX3 "data\\TEXTURE\\enemy3.png"
#define ENEMY_TEX4 "data\\TEXTURE\\enemy4.png"
#define MAX_ENEMY (128)//�G�̍ő吔
#define NUM_ENEMY (4)//�G�̎��
#define ENEMY_WIDTH (150)//��
#define ENEMY_HEIGHT (150)//����
#define ENEMY_WIDTH2 (300)//��
#define ENEMY_HEIGHT2 (300)//����
#define ENEMY_WIDTH3 (450)//��
#define ENEMY_HEIGHT3 (336)//����
#define ENEMY_WIDTH4 (560)//��
#define ENEMY_HEIGHT4 (600)//����
#define ENEMY_LIFE (9)//�̗�
#define ENEMY_LIFE2 (99)//�̗�
#define ENEMY_LIFE3 (999)//�̗�
#define ENEMY_LIFE4 (9999)//�̗�

#define ENEMY_SPEED_DEF (0.0f)//��{�X�s�[�h
#define ENEMY_MAX_SPEED (50.0f)//����
#define ENEMY_JUMP (-30.0f)//�W�����v��
#define ENEMY_JUMP_NUM (1)//�W�����v��
#define ENEMY_JUMP_MOVE_INA (0.5f)//�W�����v�ړ�����
#define ENEMY_INA (0.3f)//����

#define HIT_VECTOR (50.0f)

//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,//�ʏ�
	ENEMYSTATE_DAMAGE,//�_���[�W
	ENEMYSTATE_MAX,//��
}ENEMYSTATE;

//�G�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 Oldpos;//�O�t���[����pos
	D3DXVECTOR3 move;//�v���C���[�X�s�[�h�Ǘ�
	int nType;//���
	ENEMYSTATE state;//���
	int nCounterState;//��ԃJ�E���^�[
	float fLength;//�傫��
	float fLengthDef;//�Ίp���̒���(�傫��)�̏����l
	int nLife;//�̗�
	bool bMove;//�����Ă��邩
	bool bJump;//�W�����v���Ă��邩
	int JumpCount;
	bool bLeft;
	bool bDisp;
	bool bUse;//�����Ă��邩
	Block* pBlock;
}Enemy;

void InitEnemy(void);//�G�̏���������
void UninitEnemy(void);//�G�̏I������
void UpdateEnemy(void);//�G�̍X�V����
void DrawEnemy(void);//�G�̕`�揈��
void SetEnemy(D3DXVECTOR3 pos, int nType);//�G�ݒ�
void HitEnemy(int nCntEnemy, int nDamage, D3DXVECTOR3 pos);//�G�q�b�g
Enemy* GetEnemy(void);//�G���擾
int GetNumEnemy(void);//�G�̐��擾

void UpdateEditEnemy(void);

#endif _ENEMY_H_

