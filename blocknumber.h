//-------------------------------------- -
//
//�w�i�\�������̒�`�E�錾[back.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BACK_H_
#define _BACK_H_

#include"main.h"

#define NUM_MAX (3)
#define SIZE_INA (0.5f)

void InitBlockNumber(void);//�w�i�̏���������
void UninitBlockNumber(void);//�w�i�̏I������
void UpdateBlockNumber(void);//�w�i�̍X�V����
void DrawBlockNumber(void);//�w�i�̕`�揈��
int DigitBN(int nData, int nDigit);

#endif _BACK_H_