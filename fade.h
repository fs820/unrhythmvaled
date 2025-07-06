//------------------------------------------
//
//�t�F�[�h�����̒�`�E�錾[fade.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _FADE_H_
#define _FADE_H_

#include"main.h"

//�t�F�[�h�̏��
typedef enum
{
	FADE_NONE=0,//�ʏ�
	FADE_IN,//�t�F�[�h�C��
	FADE_OUT,//�t�F�[�h�A�E�g
	FADE_MAX//��
}FADE;

void InitFade(MODE modeNext);//�t�F�[�h����������
void UninitFade(void);//�t�F�[�h�I������
void UpdateFade(void);//�t�F�[�h�X�V����
void DrawFade(void);//�t�F�[�h�`�揈��
void SetFade(MODE modeNext);//�t�F�[�h�ݒ�
void SetFade2(MODE modeNext);
FADE GetFade(void);//�t�F�[�h��Ԏ擾

#endif  _FADE_H_