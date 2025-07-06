//---------------------------------------
//
//�A�C�e���\�������̒�`�E�錾[item.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _ITEM_H_
#define _ITEM_H_

#include"main.h"

#define ITEM_TEX "data\\TEXTURE\\item.png"
#define MAX_ITEM (128)//�ő吔
#define ITEM_SIZE (25)
#define ITEM_LIFE (1200)

//�A�C�e���^�C�v�̗񋓌^��`
typedef enum
{
	TYPE_NORMAL = 0,
	TYPE_GOAL,
	TYPE_MAX
}TYPE;

//�A�C�e���\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	TYPE Type;
	bool bDisp;
	bool bUse;//�g�p���Ă��邩�ǂ���
}Item;

void InitItem(void);//�G�t�F�N�g�̏���������
void UninitItem(void);//�G�t�F�N�g�̏I������
void UpdateItem(void);//�G�t�F�N�g�̍X�V����
void DrawItem(void);//�G�t�F�N�g�̕`�揈��
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move,TYPE Type);//�G�t�F�N�g�ݒ�
Item* GetItem(void);
bool Goal(void);

#endif _ITEM_H_
