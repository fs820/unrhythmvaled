//---------------------------------------
//
//�e�\�������̒�`�E�錾[Block.h]
//Author fuma sato
//
//---------------------------------------

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include"main.h"

#define MAX_BLOCK (256)//�ő吔
#define BLOCK_TEX "data\\TEXTURE\\block000.png"
#define BLOCK_WIDTH_DEF (80.0f)
#define BLOCK_HEIGHT_DEF (80.0f)
#define FIELD_HEIGHT (80.0f)
#define BLOCK_INA (8.0f)
#define BLOCK_SPEEDX (3.0f)
#define BLOCK_SPEEDY (-3.0f)
#define CLLISION_INA (0.1f)
#define CLLISION_NUM (10)

//�e�\����
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 move;//����
	float fWidth;//�傫��
	float fHeight;//����
	bool bDisp;
	bool bUse;//�g�p���Ă��邩�ǂ���
}Block;

void InitBlock(void);//����������
void UninitBlock(void);//�I������
void UpdateBlock(void);//�X�V����
void DrawBlock(void);//�`�揈��
void SetBlock(D3DXVECTOR3 pos, float fWidth, float fHeight);//�ݒ�
Block* GetBlock(void);
bool CllisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fWidth, float fHeight, Block** pBlock, bool bPlayer); //�����蔻��
void CllisionBlockA(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, float fLength, bool* pUse); //�����蔻��
#endif _BLOCK_H_
