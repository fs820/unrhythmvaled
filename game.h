//------------------------------------------
//
//�Q�[�����C���̏����̒�`�E�錾[game.h]
//Author fuma sato
//
//------------------------------------------

#ifndef _GAME_H_
#define _GAME_H_

#include"main.h"

#define TIME_LIMIT (90)
#define INTER (60)//�I���P�\
#define SCROLL_X (3.0f)
#define SCROLL_Y (-1.0f)

#define EDIT_BLOCK_SPACE (80.0f)

//�Q�[����Ԃ̗񋓌^��`
typedef enum
{
	GAMESTATE_NONE=0,//�Q�[���łȂ�
	GAMESTATE_NORMAL,//�Q�[����
	GAMESTATE_PAUSE,//�|�[�Y��
	GAMESTATE_END,//�I��
	GAMESTATE_MAX//��
}GAMESTATE;

void InitGame(void);//�Q�[���̏���������
void UninitGame(void);//�Q�[���̏I������
void UpdateGame(void);//�Q�[���̍X�V����
void DrawGame(void);//�Q�[���̕`�揈��
void SetGameState(GAMESTATE state);//�Q�[���̏�Ԑݒ�
GAMESTATE GetGameState(void);//�Q�[����Ԏ擾
bool GetClear(void);
bool Edit(void);

//�f�o�b�N�p
void EditMode(void);
#endif _GAME_H_
