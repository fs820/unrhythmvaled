//---------------------------------------
//
//�J�[�\�������̒�`�E�錾[cursor.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include"main.h"

#define TEXTURE_CURSOR "data\\TEXTURE\\crown_king_queen_icon_231318.png"

//�v���C���[���
#define CURSOR_WIDTH (64)//��
#define CURSOR_HEIGHT (64)//����
#define CURSOR_SPEED_DEF (0.0f)//�����X�s�[�h
#define CURSOR_SPEED_X (20.0f)//����
#define CURSOR_SPEED_Y (20.0f)//����

#define U_MAX_CU (1)//�e�N�X�`���̐�(��)
#define V_MAX_CU (1)//�e�N�X�`���̐�(�c)

//�J�[�\����Ԃ̗񋓌^��`
typedef enum
{
	CURSORSTATE_NONE = 0,//��\��
	CURSORSTATE_NORMAL,//�ʏ�
	CURSORSTATE_POINT,//�H
	CURSORSTATE_MAX//��ސ�
}CURSORSTATE;

//�J�[�\���\����
typedef struct
{
	int nCounterAnim;//�J�[�\���̍X�V�Ԋu
	int nPatternAnim;//�J�[�\���̃A�j���[�V�����p�^�[���Ǘ�
	D3DXVECTOR3 pos;//�J�[�\�����W�Ǘ�
	D3DXVECTOR3 move;//�J�[�\���X�s�[�h�Ǘ�
	CURSORSTATE state;//���
	bool bUse;//�g�p���Ă��邩
}CURSOR;

void InitCursor(void);//�J�[�\���̏���������
void UninitCursor(void);//�J�[�\���̏I������
void UpdateCursor(void);//�J�[�\���̍X�V����
void DrawCursor(void);//�J�[�\���̕`�揈��
PDIRECT3DVERTEXBUFFER9 GetCursorBuffer(void);//�J�[�\���o�b�t�@�̎擾
CURSOR* GetCursorIn(void);//�J�[�\�����擾
void CrCursor(bool bUse);//�\���ؑ�

#endif _CURSOR_H_
