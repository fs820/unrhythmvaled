//---------------------------------------
//
//�X�R�A�����̒�`�E�錾[time.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _TIME_H_
#define _TIME_H_

#include"main.h"

#define TIME_MAX (2)//����
#define U_MAX_S (10)//�e�N�X�`����(��)
#define V_MAX_S (1)//�e�N�X�`����(�c)
#define TIME_WIDTH (350.0f)//�T�C�Y
#define TIME_HEIGHT (70.0f)//�T�C�Y
#define DIGIT (10)//�i��

//�v���g�^�C�v�錾
void InitTime(void);//�X�R�A�̏���������
void UninitTime(void);//�X�R�A�̏I������
void UpdateTime(void);//�X�R�A�̍X�V����
void DrawTime(void);//�X�R�A�̕`�揈��
void SetTime(int nTime, bool bTex);//�X�R�A��������
int DigitT(int nData, int nDigit);

#endif _Time_H_
