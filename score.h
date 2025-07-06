//---------------------------------------
//
//�X�R�A�����̒�`�E�錾[score.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

#define SCORE_MAX (10)//����
#define U_MAX_S (10)//�e�N�X�`����(��)
#define V_MAX_S (1)//�e�N�X�`����(�c)
#define SCORE_WIDTH (350.0f)//�T�C�Y
#define SCORE_HEIGHT (70.0f)//�T�C�Y
#define DIGIT (10)//�i��

#define SCORE_HIT (100)//�G�ɓ���������
#define SCORE_DES (1000)//�G��|������
#define SCORE_HIT_BOSS (1000)//�{�X�ɓ���������
#define SCORE_DES_BOSS (100000)//�{�X��|�����Ƃ�
#define SCORE_MISS (1000)//�_���[�W���󂯂���
#define SCORE_DIE (10000)//���񂾂Ƃ�
#define SCORE_ITEM (1000000)
#define SCORE_GOAL (5000000)

//�v���g�^�C�v�錾
void InitScore(void);//�X�R�A�̏���������
void UninitScore(void);//�X�R�A�̏I������
void UpdateScore(void);//�X�R�A�̍X�V����
void DrawScore(void);//�X�R�A�̕`�揈��
void SetScore(int nScore,bool bTex);//�X�R�A��������
void AddScore(int nValue);//�X�R�A���Z
int GetScore(void);//�X�R�A�擾
int Digit(int nData, int nDigit);//��������

#endif _SCORE_H_
